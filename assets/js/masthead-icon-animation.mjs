function getIcons() {
  const iconNames = [
    "icon_posts", "icon_download", "icon_guide", "icon_play", "icon_ref"
  ];
  var icons = [];
  for (const name of iconNames) {
    icons.push(document.getElementById(name));
  }
  return icons;
}

export function animateNaviIcon() {
  const animations = ["animation-full-circle", "animation-pop", "animation-rotate-vertical"];
  const icons = getIcons();
  for (const icon of icons) {
    for (const ani of animations) {
      icon.classList.remove(ani);
    }
  }

	var diceroll = Math.floor(Math.random() * (icons.length));
  var animationN = Math.floor(Math.random() * (animations.length));
  icons[diceroll].classList.add(animations[animationN]);

  let timeout = Math.floor(Math.random() * 4000) + 1500; // 3s ~ max 23s.
  setTimeout(animateNaviIcon, timeout);
}