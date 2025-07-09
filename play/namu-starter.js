let examples = {
  'hello-world': `# Hello, I'm Namu language.
# Please edit your code freely and press button in the top right to run.
# Refer to the 'guide' section above for more syntax.
main() void
    print("hello world!")`,

  'bow-to-everyone': `def person
    name := "default"
    bow() str
        print("hello. I'm " + name + "\\n")
        name

main() void
    mike := person()
    mike.name = "mike"

    last := for p in {mike, mike(), mike(mike)}
        p.bow()

    print("and second one was " + last[1])`
};

var isEditingYourCodes = true;

function updateCode(code) {
    document.getElementById('ta_codepad').value = code;
    highlight(document.getElementById("ta_codepad"))
}

function loadStorage() {
  if(typeof(Storage) == undefined) return;

  var src = localStorage.getItem("src") || examples['hello-world']
  updateCode(src);
}

function saveStorage() {
  if(typeof(Storage) == undefined) return;

  let src = document.getElementById('ta_codepad').value;
  localStorage.setItem("src", src);
}

window.addEventListener('resize', function(event) {
  resizeCodePad();
}, true);

window.onbeforeunload = function() {
  if(isEditingYourCodes) saveStorage();
}

function highlight(ta) {
    let highlighted = hljs.highlight(ta.value, { language: "byeol"}).value
    document.getElementById("pre_codepad").innerHTML = highlighted;
}
function inputTab(textArea) {
    if (event.key == 'Tab') {
        event.preventDefault();
        var start = textArea.selectionStart;
        var end = textArea.selectionEnd;

        textArea.value = textArea.value.substring(0, start) + "    " + textArea.value.substring(end);

        // put caret at right position again
        textArea.selectionStart = textArea.selectionEnd = start + 4;
      }
}

function runScript(frameId, srcId) {
    var bottom = document.getElementById('result');
    bottom.style.display = 'block';

    runNamuTextArea(frameId, srcId);
    showCloseButton();
    resizeCodePad();
}

function showCloseButton() {
    const close = document.getElementById('bt-close');
    close.style.setProperty('display', 'block');
}

function onclickCloseButton() {
    const close = document.getElementById('bt-close');
    close.style.setProperty('display', 'hidden');

    var bottom = document.getElementById('result');
    bottom.style.display = 'none';
    resizeCodePad();
}

function resizeCodePad() {
    let ta = document.getElementById("ta_codepad");
    let pre = document.getElementById("pre_codepad");
    let preStyle = window.getComputedStyle(document.getElementById('pre_codepad'))
    let top_pane = document.getElementById("top_pane");

    let top_height = top_pane.offsetTop + top_pane.offsetHeight;
    let bottom_height = document.getElementById("footer")?.offsetHeight || 0;
    let window_height = window.innerHeight;
    let result_height = window.innerWidth <= 950 ? document.getElementById('result').offsetHeight : 0;
    let codepad_height = window_height - (top_height + bottom_height + result_height) + "px";

    ta.style.maxHeight = codepad_height;
    ta.style.height = codepad_height;
    pre.style.maxHeight = codepad_height;
    pre.style.height = codepad_height;
    ta.style.width = pre.offsetWidth + "px";
}

function onchangeSrc(value) {
    if(value == "your-codes") {
      isEditingYourCodes = true;
      return loadStorage();
    }

    if(isEditingYourCodes) saveStorage();
    isEditingYourCodes = false;

    let example = examples[value];
    if(example == undefined) return;

    updateCode(example);
}

window.onload = function() {
  loadStorage();
  resizeCodePad();

  var ta = document.getElementById('ta_codepad');
  var pre = document.getElementById('pre_codepad');
  document.getElementById("ta_codepad").addEventListener('scroll', (e) => pre.scrollTop = ta.scrollTop, false);
}

function runNamuSrc(iframeId, src) {
    var iframe = document.getElementById(iframeId);
    var win = iframe.contentWindow;
    iframe.onload = function() {
        win.postMessage(src, '*');
    }
    win.location.reload();
}
function runNamuTextArea(iframe, srcId) {
    var src = document.getElementById(srcId);
    runNamuSrc(iframe, src.value);
}
function runNamuDiv(postfix) {
    var src = document.getElementById("ta"+postfix);
    var play = document.getElementById("bt"+postfix);
    runNamuSrc("fr"+postfix, src.textContent);
    play.style.display = 'none';
}