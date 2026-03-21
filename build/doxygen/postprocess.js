followDarkModeWithParent();

document.addEventListener('DOMContentLoaded', function() {
    function makeJson(raw) {

        // prevent javascript handling string template or escape sequence append
        // let it pass string literal to my wasm binary
        function wrapEscapeSequence(rawStr) {
            const pattern = /(##[\s\S]*?##)|(#.*)|("(?:[^"\\]|\\.)*"|'(?:[^'\\]|\\.)*')/g

            return rawStr.replace(pattern, (match, multiComment, singleComment, string) => {
                if(multiComment || singleComment) return match;
                if(!string) return match;

                return string
                    .replace(/\\/g, '\\\\')
                    .replace(/\$\{/g, '\\${')
                    .replace(/\n/g, '\\n')
                    .replace(/\t/g, '\\t')
                    .replace(/\\n/g, '\\n')
                    .replace(/\\t/g, '\\t');
            });
        }

        function emptyJson(raw, styles) {
            if(style == null) {
                return {
                    code: raw.replace(/\"/g, '$quot;'),
                    shown: raw,
                    style: `language-cpp verified`
                };
            }

            return {
                code: raw.replace(/\"/g, '$quot;'),
                shown: raw,
                style: styles
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
        codes = wrapEscapeSequence(raw);
        if(style != null) {
            rows.shift();
            codes = rows.join("\n");
        }
        if(codes[0] != '{') return emptyJson(codes, style);

        try {
            return eval(`(${codes})`);
        } catch (ex) {
            console.log(ex.stack);
            return emptyJson(codes, style);
        }
    }

    // preprocess: making tags as attributes.
    const fragments = document.querySelectorAll('.fragment, div.fragment, pre.fragment');
    fragments.forEach(fragment => {
        let json = makeJson(fragment.textContent);
        // in default, each code block expands to fragment with lots of <div> representing a line.
        // so, my first move is to empty all of divs and fill it new <code> tag.
        fragment.innerHTML = "";
        const pre = document.createElement('pre');
        fragment.append(pre);
        let code = document.createElement('code');
        pre.append(code);
        // now styling code tag:
        code.textContent = json.shown;
        code.classList.add('hljs', ...json.style.split(' '));
        code.setAttribute('src', json.code);
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
