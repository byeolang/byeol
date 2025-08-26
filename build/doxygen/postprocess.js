followDarkModeWithParent();

document.addEventListener('DOMContentLoaded', function() {
    const fragments = document.querySelectorAll('.fragment, div.fragment, pre.fragment');
    fragments.forEach(fragment => {
        let code = fragment.textContent;
        fragment.innerHTML = '<pre><code class="language-byeol">' + code + '</code></pre>';
    });
});

window.addEventListener('load', function() {
    const codeBlocks = document.querySelectorAll('pre code.language-byeol');
    codeBlocks.forEach(block => {
        hljs.highlightElement(block);
    });
});

function followDarkModeWithParent() {
    let parentHtml = window.parent.document.getElementsByTagName("html")[0];
    let isDarkMode = parentHtml.classList.contains("dark-mode");
    let className = isDarkMode ? "dark-mode" : "light-mode"
    let myHtml = document.getElementsByTagName('html')[0];
    myHtml.classList.add(className);
}
