import csv
from sys import argv, exit


def main():

    # Check for command-line usage
    if len(argv) != 3:
        print(f"Usage: python {argv[0]} data.csv sequence.txt")
        exit(1)

    # Read database file into a variable
    with open(argv[1], "r") as file:
        reader = csv.DictReader(file)
        database = list(reader)
        str_names = reader.fieldnames[1:]  # skip "name"

    # Read DNA sequence file into a variable
    with open(argv[2], "r") as file:
        sequence = file.read().strip()

    # Find longest match of each STR in DNA sequence
    str_counts = {}
    for s in str_names:
        str_counts[s] = longest_match(sequence, s)

    # Check database for matching profiles
    for person in database:
        for s in str_names:
            if int(person[s]) != str_counts[s]:
                break
        else:
            print(person["name"])
            return

    print("No match")


def longest_match(sequence: str, subsequence: str) -> int:
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in sequence, return longest run found
    return longest_run


if __name__ == "__main__":
    main()
