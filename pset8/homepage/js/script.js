// Username to fetch user data
const githubUsername = 'ztrahmet';

// Sidebar tabs mapping
const pages = [
    {file: 'index.html', name: 'Home'},
    {file: 'about.html', name: 'About'},
    {file: 'projects.html', name: 'Projects'},
    {file: 'contact.html', name: 'Contact'}
]

document.addEventListener('DOMContentLoaded', () => {
    // Find or create sidebar
    const sidebar = document.querySelector('.sidebar');
    if (!sidebar) return; // Exit if sidebar doesn't exist

    // ---------- Profile ----------
    const profileLink = document.createElement('a');
    profileLink.href = `https://github.com/${githubUsername}`;
    profileLink.classList.add('profile-container');

    // Create profile elements
    const imgEl = document.createElement('img');
    imgEl.classList.add('profile-avatar');
    imgEl.alt = 'Avatar';

    const textDiv = document.createElement('div');
    textDiv.classList.add('profile-text');

    const fullNameEl = document.createElement('p');
    fullNameEl.classList.add('profile-full-name');

    const usernameEl = document.createElement('p');
    usernameEl.classList.add('profile-username');

    textDiv.append(fullNameEl, usernameEl);
    profileLink.append(imgEl, textDiv);
    sidebar.appendChild(profileLink);

    // Fetch GitHub profile
    fetch(`https://api.github.com/users/${githubUsername}`)
        .then(res => res.json())
        .then(data => {
            const fullName = data.name;
            const avatar = data.avatar_url;

            // Set the values of profile elements
            imgEl.src = avatar;
            fullNameEl.textContent = fullName;
            usernameEl.textContent = `@${githubUsername}`;

            // ---------- Copyright ----------
            const notice = document.createElement('div');
            notice.classList.add('copyright');
            notice.textContent = `© ${new Date().getFullYear()} ${fullName}`;
            sidebar.appendChild(notice);
        })
        .catch(err => console.error('Error fetching GitHub profile:', err));

    // ---------- Links ----------
    const sidebarLinksDiv = document.createElement('div');
    sidebarLinksDiv.classList.add('sidebar-links');
    sidebar.appendChild(sidebarLinksDiv);

    // Get current page link
    const currentPage = window.location.pathname.split('/').pop();

    // Create links/tabs
    pages.forEach(page => {
        const linkEl = document.createElement('a');
        linkEl.href = page.file;
        linkEl.textContent = page.name;

        // Create class for current link
        if (page.file === currentPage) linkEl.classList.add('current');

        // Add link to links
        sidebarLinksDiv.appendChild(linkEl);
    });
});
