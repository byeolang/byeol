followDarkModeWithParent();

document.addEventListener('DOMContentLoaded', function() {
    function makeJson(raw) {
        function emptyJson(raw) {
            return {
                code: raw,
                shown: raw,
                classList: ""
            };
        }

        if(raw[0] != '{') return emptyJson(raw);
        try {
            return eval(`(${raw})`);
        } catch (ex) {
            return emptyJson(raw);
        }
    }

    function convertNewLine(shown) {
        return shown.replace(/\\n/g, '\n');
    }

    // preprocess: making tags as attributes.
    const fragments = document.querySelectorAll('.fragment, div.fragment, pre.fragment');
    fragments.forEach(fragment => {
        let json = makeJson(fragment.textContent);
        fragment.innerHTML = `<pre><code class="language-byeol ${json.classList}" src="${json.code}">${json.shown}</code></pre>`;
    });
});

window.addEventListener('load', function() {
    const codeBlocks = document.querySelectorAll('pre code.language-byeol');
    codeBlocks.forEach(block => {
        hljs.highlightElement(block);
    });

    $('code.hljs.verified').hover(function() {
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
