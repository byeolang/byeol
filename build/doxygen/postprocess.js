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

    $('code.language-byeol').each(function(index) {
        $(this).attr("src", this.textContent)
    })
    $('code.hljs').hover(function() {
        var codeTag = $(this)[0]
        if($(this).find(".play_button").length) return
        $(this).append('<a class="play_button"><span/></a>')
        $(this).find('a.play_button').click(function() {
          redirectPlay(codeTag)
        })
    }, function() {
      $('.play_button').remove()
    })
});

function redirectPlay(codeTag) {
    var src = $(codeTag).attr("src")
    window.localStorage.setItem("src", src)
    window.open("/play", "_blank")
}

function followDarkModeWithParent() {
    let parentHtml = window.parent.document.getElementsByTagName("html")[0];
    let isDarkMode = parentHtml.classList.contains("dark-mode");
    let className = isDarkMode ? "dark-mode" : "light-mode"
    let myHtml = document.getElementsByTagName('html')[0];
    myHtml.classList.add(className);
}
