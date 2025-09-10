import os
from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    user_id = session["user_id"]

    # Sum shares per symbol (buys positive, sells negative)
    rows = db.execute(
        "SELECT symbol, SUM(shares) AS shares FROM transactions WHERE user_id = ? GROUP BY symbol HAVING shares > 0",
        user_id,
    )

    holdings = []
    total_stocks_value = 0.0

    for row in rows:
        symbol = row["symbol"]
        shares = row["shares"]
        quote = lookup(symbol)
        if quote is None:
            # If lookup failed, skip symbol (or set price 0)
            price = 0.0
            name = symbol
        else:
            price = float(quote["price"])
            name = quote["name"]
        total = shares * price
        total_stocks_value += total
        holdings.append(
            {
                "symbol": symbol,
                "name": name,
                "shares": shares,
                "price": price,
                "total": total,
            }
        )

    # Get cash balance from users table
    rows = db.execute("SELECT cash FROM users WHERE id = ?", user_id)
    cash = float(rows[0]["cash"]) if rows else 0.0

    grand_total = cash + total_stocks_value

    return render_template(
        "index.html", holdings=holdings, cash=cash, grand_total=grand_total
    )


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    user_id = session["user_id"]

    if request.method == "GET":
        return render_template("buy.html")

    # POST
    symbol = request.form.get("symbol")
    shares_str = request.form.get("shares")

    if not symbol:
        return apology("must provide symbol", 400)

    # shares must be a positive integer
    try:
        shares = int(shares_str)
        if shares <= 0:
            raise ValueError
    except Exception:
        return apology("shares must be a positive integer", 400)

    quote = lookup(symbol)
    if quote is None:
        return apology("invalid symbol", 400)

    price = float(quote["price"])
    cost = shares * price

    # Check user's cash
    rows = db.execute("SELECT cash FROM users WHERE id = ?", user_id)
    cash = float(rows[0]["cash"])

    if cost > cash:
        return apology("can't afford", 400)

    # Deduct cash and record transaction (buy => positive shares)
    new_cash = cash - cost
    db.execute("UPDATE users SET cash = ? WHERE id = ?", new_cash, user_id)

    db.execute(
        "INSERT INTO transactions (user_id, symbol, shares, price, transacted) VALUES (?, ?, ?, ?, CURRENT_TIMESTAMP)",
        user_id,
        symbol.upper(),
        shares,
        price,
    )

    flash(f"Bought {shares} shares of {symbol.upper()} at {usd(price)} each")
    return redirect("/")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    user_id = session["user_id"]
    rows = db.execute(
        "SELECT symbol, shares, price, transacted FROM transactions WHERE user_id = ? ORDER BY transacted DESC",
        user_id,
    )

    # show symbol, shares (positive for buy, negative for sell), price, transacted
    return render_template("history.html", transactions=rows)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "GET":
        return render_template("quote.html")

    symbol = request.form.get("symbol")
    if not symbol:
        return apology("must provide symbol", 400)

    quote = lookup(symbol)
    if quote is None:
        return apology("invalid symbol", 400)

    # Show quote result
    return render_template(
        "quoted.html", name=quote["name"], price=quote["price"], symbol=quote["symbol"]
    )


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "GET":
        return render_template("register.html")

    username = request.form.get("username")
    password = request.form.get("password")
    confirmation = request.form.get("confirmation")

    # Validate
    if not username:
        return apology("must provide username", 400)
    if not password:
        return apology("must provide password", 400)
    if password != confirmation:
        return apology("passwords do not match", 400)

    # Check username not taken
    rows = db.execute("SELECT * FROM users WHERE username = ?", username)
    if len(rows) > 0:
        return apology("username already taken", 400)

    hash_pw = generate_password_hash(password)
    # Initial cash as in CS50 spec (typically $10,000)
    db.execute("INSERT INTO users (username, hash, cash) VALUES (?, ?, ?)", username, hash_pw, 10000.00)

    # Log user in
    rows = db.execute("SELECT id FROM users WHERE username = ?", username)
    session["user_id"] = rows[0]["id"]

    flash("Registered!")
    return redirect("/")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    user_id = session["user_id"]

    if request.method == "GET":
        # Provide list of symbols user currently owns
        rows = db.execute(
            "SELECT symbol, SUM(shares) AS shares FROM transactions WHERE user_id = ? GROUP BY symbol HAVING shares > 0",
            user_id,
        )
        # row["symbol"] for options
        return render_template("sell.html", symbols=[r["symbol"] for r in rows])

    # POST
    symbol = request.form.get("symbol")
    shares_str = request.form.get("shares")

    if not symbol:
        return apology("must provide symbol", 400)

    try:
        shares = int(shares_str)
        if shares <= 0:
            raise ValueError
    except Exception:
        return apology("shares must be a positive integer", 400)

    # Check current owned shares
    rows = db.execute(
        "SELECT SUM(shares) AS shares FROM transactions WHERE user_id = ? AND symbol = ? GROUP BY symbol",
        user_id,
        symbol,
    )
    owned = rows[0]["shares"] if rows else 0
    if owned is None:
        owned = 0

    if shares > owned:
        return apology("too many shares", 400)

    quote = lookup(symbol)
    if quote is None:
        return apology("invalid symbol", 400)

    price = float(quote["price"])
    proceeds = shares * price

    # Add cash to user
    rows = db.execute("SELECT cash FROM users WHERE id = ?", user_id)
    cash = float(rows[0]["cash"])
    new_cash = cash + proceeds
    db.execute("UPDATE users SET cash = ? WHERE id = ?", new_cash, user_id)

    # Record transaction as negative shares
    db.execute(
        "INSERT INTO transactions (user_id, symbol, shares, price, transacted) VALUES (?, ?, ?, ?, CURRENT_TIMESTAMP)",
        user_id,
        symbol.upper(),
        -shares,
        price,
    )

    flash(f"Sold {shares} shares of {symbol.upper()} at {usd(price)} each")
    return redirect("/")
