
export function showCloseButton() {
    const close = document.getElementById('bt-close');
    close.style.setProperty('display', 'block');
}

export function onClickCloseButton() {
    const close = document.getElementById('bt-close');
    close.style.setProperty('display', 'hidden-popup');

    var result = document.getElementById('result');
    result.classList.add('hidden-popup');
    result.style.opacity = '0';
    resizeCodePad();
}

export function showPopup() {
    const result = document.getElementById('result');
    result.style.display = 'block';
    result.style.opacity = '0.8';
    result.classList.remove('hidden-popup');

    onResizePopup()
}

export function onResizePopup() {
    const winWidth = window.innerWidth;
    const winHeight = window.innerHeight;
    const codepad = document.getElementById('codepad');
    const rect = codepad.getBoundingClientRect();
    const popup = document.getElementById('result');
    const padding = 50;

    if (winWidth > 950) {
      const width = 500;
      const height = winHeight - (rect.top + padding * 2);

      popup.style.left = (winWidth - (padding + 500)) + 'px';
      popup.style.top = (rect.top + padding) + 'px';
      popup.style.width = width + 'px';
      popup.style.height = height + 'px';

    } else {
      const width = winWidth - padding * 2;
      const height = 250;

      popup.style.left = padding + 'px';
      popup.style.top = (winHeight - (height + padding)) + 'px';
      popup.style.width = width + 'px';
      popup.style.height = height + 'px';
    }
}