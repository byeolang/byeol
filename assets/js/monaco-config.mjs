import * as monaco from 'https://cdn.jsdelivr.net/npm/monaco-editor@0.52.2/+esm';
import { DoxygenAwesomeDarkModeToggle } from './doxygen-awesome-darkmode-toggle.mjs';
import { byeolLanguage } from './monaco-editor-byeol-language-min.mjs';

monaco.languages.register({id: 'byeol'});
monaco.languages.setMonarchTokensProvider('byeol', byeolLanguage);

const colors = {
  white: "#FFFFFF",
  white2: "#E9EBF2",
  night: "#0B1C26",
  night2: "#112A40",
  night3: "#2E4559",
  grass: "#6AB04B",
  point: "#4189A6",
  moon: "#E0B88D",
  nebular: "#CA5FA4",
  nebular2: "#F2B6D7",
}

monaco.editor.defineTheme('byeolTheme-dark', {
  base: 'vs-dark',
  inherit: false,
  rules: [
    { token: "keyword", foreground: colors.point },
    { token: "comment", foreground: colors.night3 },
    { token: "number", foreground: colors.white2 },
    { token: "string", foreground: colors.nebular },
    { token: "delimiter", foreground: colors.night3 },
    { token: "operator", foreground: colors.night3 },
    { token: "identifier", foreground: colors.moon },
    { token: "type", foreground: colors.grass },
  ],
  colors: {
    'editor.background': colors.night,
    "scrollbar.shadow": "#00000000",
    "scrollbarSlider.activeBackground": colors.point,
    "scrollbarSlider.background": colors.night2,
    "scrollbarSlider.hoverBackground": colors.point,
  }
});

monaco.editor.defineTheme('byeolTheme-light', {
  base: 'vs',
  inherit: false,
  rules: [
    { token: "keyword", foreground: colors.point },
    { token: "comment", foreground: '#999999' },
    { token: "number", foreground: colors.night3 },
    { token: "string", foreground: colors.nebular },
    { token: "delimiter", foreground: colors.white2 },
    { token: "operator", foreground: colors.white2 },
    { token: "identifier", foreground: colors.moon },
    { token: "type", foreground: colors.grass },
  ],
  colors: {
    'editor.background': colors.white,
    "scrollbar.shadow": "#00000000",
    "scrollbarSlider.activeBackground": colors.point,
    "scrollbarSlider.background": colors.white2,
    "scrollbarSlider.hoverBackground": colors.point,
  }
});

export const editor = monaco.editor.create(document.getElementById('codepad'), {
  language: 'byeol',
  minimap: { enabled: false },
  renderLineHighlight: "none",
  automaticLayout: true,
  fontFamily: 'Retro Gaming',
  fontSize: '20',
  scrollbar: {
    horizontal: 'hidden',
    verticalScrollbarSize: 10,
  }
});

DoxygenAwesomeDarkModeToggle.onDarkModeChange = function(isEnable) {
  if (isEnable) {
      monaco.editor.setTheme('byeolTheme-dark');
  } else {
      monaco.editor.setTheme('byeolTheme-light');
  }
}

export const model = editor.getModel();