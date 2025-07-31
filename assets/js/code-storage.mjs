import { examples } from './examples.mjs';
import { model } from './monaco-config.mjs'

export function loadStorage() {
  if(typeof(Storage) == undefined) return;

  var src = localStorage.getItem("src") || examples['hello-world']
  model.setValue(src);
}

export function saveStorage() {
  if(typeof(Storage) == undefined) return;

  let src = model.getValue()
  localStorage.setItem("src", src);
}