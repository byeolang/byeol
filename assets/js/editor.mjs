import { examples } from './examples.mjs';
import { editor, model } from './monaco-config.mjs'
import { showCloseButton, onClickCloseButton, showPopup, onResizePopup } from './result-popup.mjs'
import { loadStorage, saveStorage } from './code-storage.mjs'

let isEditingYourCodes = true;

function onClickRunButton() {
    showPopup()
    const code = model.getValue()
    run(code);
    showCloseButton();
    resizeCodePad();
}

function resizeCodePad() {
    let top_pane = document.getElementById("top_pane");
    let top_height = top_pane.offsetTop + top_pane.offsetHeight;
    let bottom_height = document.getElementById("footer")?.offsetHeight || 0;
    let window_height = window.innerHeight;
    let newHeight = window_height - (top_height + bottom_height) + "px";

    const codepad = document.getElementById('codepad');
    codepad.style.height = newHeight;
    codepad.style.maxHeight = newHeight;
    editor.layout();

    onResizePopup()
}

function onChangeSrc(option) {
    const value = option.target.value;

    if(value == "your-codes") {
      isEditingYourCodes = true;
      return loadStorage();
    }

    if(isEditingYourCodes) saveStorage();
    isEditingYourCodes = false;

    const example = examples[value];
    if(example == undefined) return;

    model.setValue(example);
}

function attachListeners() {
  document.getElementById('dd-srcs').addEventListener('change', onChangeSrc);
  document.getElementById('runbt').addEventListener('click', onClickRunButton);
  document.getElementById('bt-close').addEventListener('click', onClickCloseButton);
}

window.onload = function() {
  attachListeners()
  loadStorage();
  resizeCodePad();
}

window.addEventListener('resize', function(event) {
  resizeCodePad();
}, true);

window.onbeforeunload = function() {
  if(isEditingYourCodes) saveStorage();
}

function run(src) {
    var iframe = document.getElementById('byeol');
    var win = iframe.contentWindow;
    iframe.onload = function() {
        win.postMessage(src, '*');
    }
    win.location.reload();
}