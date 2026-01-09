followDarkModeWithParent();

document.addEventListener('DOMContentLoaded', function() {
    function makeJson(raw) {

        function emptyJson(raw, style) {
            if(style == null) {
                return {
                    code: raw.replace(/\"/g, '$quot;'),
                    shown: raw.replace(/</g, "&lt;").replace(/>/g, "&gt;"),
                    classList: `language-cpp verified`
                };
            }

            return {
                code: raw.replace(/\"/g, '$quot;'),
                shown: raw.replace(/</g, "&lt;").replace(/>/g, "&gt;"),
                classList: style
            };
        }

        function getStyle(rows) {
            if (rows.length <= 0) return null;

            const match = rows[0].match(/^@style:\s*([^\n]+)/);
            if(match == null) return null;

            return match[1];
        }

        const rows = raw.split("\n");
        const style = getStyle(rows);
        codes = raw;
        if(style != null) {
            rows.shift();
            codes = rows.join("\n");
        }
        if(codes[0] != '{') return emptyJson(codes, style);

        try {
            return eval(`(${codes})`);
        } catch (ex) {
            return emptyJson(codes, style);
        }
    }

    function convertNewLine(shown) {
        return shown.replace(/\\n/g, '\n');
    }

    // preprocess: making tags as attributes.
    const fragments = document.querySelectorAll('.fragment, div.fragment, pre.fragment');
    fragments.forEach(fragment => {
        let json = makeJson(fragment.textContent);
        fragment.innerHTML = `<pre><code class="hljs ${json.classList}" src="${json.code}">${json.shown}</code></pre>`;
    });
});

window.addEventListener('load', function() {
    const codeBlocks = document.querySelectorAll('pre code.hljs');
    codeBlocks.forEach(block => {
        hljs.highlightElement(block);
    });

    $('code.hljs.verified.runnable').hover(function() {
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
