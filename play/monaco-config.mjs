import * as monaco from 'https://cdn.jsdelivr.net/npm/monaco-editor@0.52.2/+esm';

export const editor = monaco.editor.create(document.getElementById('codepad'), {
  language: 'javascript',
  theme: 'vs-dark',
  automaticLayout: true
});
export const model = editor.getModel();