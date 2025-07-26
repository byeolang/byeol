import * as monaco from 'https://cdn.jsdelivr.net/npm/monaco-editor@0.52.2/+esm';

monaco.languages.register({id: 'byeol'});
monaco.languages.setMonarchTokensProvider('byeol', {
  keywords: [
    'def', 'as', 'is', 'ctor', 'on', 'in', 'next', 'else',
    'for', 'if', 'ret', 'while', 'with', 'pack', 'break', 'get',
    'set', 'end', 'true', 'false'
  ],

  typeKeywords: [
    'void', 'err', 'int', 'super', 'byte', 'flt', 'str', 'char', 'me', 'it'
  ],

  operators: [
    '=', '>', '<', '!', '~', ':', '==', '<=', '>=', '!=',
    '&&', '||', '++', '--', '+', '-', '*', '/', '&', '|', '^', '%',
    '<<', '>>', '>>>', '+=', '-=', '*=', '/=', '&=', '|=', '^=',
    '%=', ':='
  ],

  // we include these common regular expressions
  symbols:  /[=><!~?:&|+\-*\/\^%\:]+/,

  // C# style strings
  escapes: /\\(?:[abfnrtv\\"']|x[0-9A-Fa-f]{1,4}|u[0-9A-Fa-f]{4}|U[0-9A-Fa-f]{8})/,

  // The main tokenizer for our languages
  tokenizer: {
    root: [
      // identifiers and keywords
      [/[a-z_$][\w$]*/, { cases: { '@typeKeywords': 'type',
                                   '@keywords': 'keyword',
                                   '@default': 'identifier' } }],
      [/[A-Z][\w\$]*/, 'type.identifier' ],  // to show class names nicely

      // whitespace
      { include: '@whitespace' },

      // delimiters and operators
      [/[{}()\[\]]/, '@brackets'],
      [/[<>](?!@symbols)/, '@brackets'],
      [/@symbols/, { cases: { '@operators': 'operator',
                              '@default'  : '' } } ],

      // numbers
      [/\d*\.\d+([eE][\-+]?\d+)?/, 'number.float'],
      [/0[xX][0-9a-fA-F]+/, 'number.hex'],
      [/\d+/, 'number'],

      // delimiter: after number because of .\d floats
      [/[,.]/, 'delimiter'],

      // strings
      [/"([^"\\]|\\.)*$/, 'string.invalid' ],  // non-teminated string
      [/"/,  { token: 'string.quote', bracket: '@open', next: '@string' } ],
    ],

    comment: [
    ],

    string: [
      [/[^\\"]+/,  'string'],
      [/@escapes/, 'string.escape'],
      [/\\./,      'string.escape.invalid'],
      [/"/,        { token: 'string.quote', bracket: '@close', next: '@pop' } ]
    ],

    whitespace: [
      [/[ \t\r\n]+/, 'white'],
      [/#[^\n#]+/, 'comment'], // one line comment
      [/##[^#]*##/, 'comment'], // multi line comment
    ],
  },
});

monaco.editor.defineTheme('byeolTheme', {
  base: 'vs-dark',
  inherit: false,
  rules: [
    { token: "keyword", foreground: "#4189A6" },
    { token: "comment", foreground: "#556d69" },
    { token: "number", foreground: "#E9EBF2" },
    { token: "string", foreground: "#CA5FA4" },
    { token: "delimiter", foreground: "#2E4559" },
    { token: "operator", foreground: "#2E4559" },
    { token: "identifier", foreground: "#b9a999" },
    { token: "type", foreground: "#6AB04B" },

  ],
  colors: {
    'editor.background': '#0B1C26'
  }
});

export const editor = monaco.editor.create(document.getElementById('codepad'), {
  language: 'byeol',
  theme: 'byeolTheme',
  minimap: { enabled: false },
  automaticLayout: true,
  fontFamily: 'Retro Gaming',
  fontSize: '20',
});
export const model = editor.getModel();