var __tide_dom_id_generator = 0;
function Tide(new_owner) {
	this.grade = Math.random() * 100;
	var life = 2.5 + (Math.random() * 3);

  let ownerObj = new_owner.getJQObject();
  let ownerWidthRatio = (ownerObj.width() / 1000) * 23;
  let baseY = ownerObj.position().top,
	    h = 1 + (this.grade * 0.05),
      w = h * ownerWidthRatio,
	    x = Math.random() * ownerObj.width() - w,
  		y = baseY + (this.grade / 100) * (ownerObj.height() * 0.5) - h - 20;

	Unit.call(this, "tide" + __tide_dom_id_generator++, "/assets/images/tide.png",
		x, y, w, h, life, new_owner, "opacity: 0; ");
}
Tide.prototype = new Unit();
Tide.prototype.getGrade = function() {
	return this.grade;
}
Tide.prototype.initialize = function() {
	Unit.prototype.initialize.call(this);

	this.opacity = 1;
	/*var total_rotation_amound = this.grade * 3.6;
	if(this.getJQObject() == undefined) return;*/

  let halfLife = this.life * 0.5
	TweenMax.to(this.getJQObject(), halfLife, {
		opacity: this.opacity,
		ease: Linear.easeNone
	});

	TweenMax.to(this.getJQObject(), halfLife, {
		delay: halfLife,
		opacity: "0"
	});

  let step = (this.grade / 100) * 5 + 1;
  let increment = step * step;
  let targetY = this.y + (30 * (this.grade / 100));
	TweenMax.to(this.getJQObject(), this.life, {
    top: targetY + "px",
    left: (this.x - increment) + "px",
    width: ((this.width) + (increment * 2)) + "px",
		ease: Linear.easeNone
	});
}

export function SeaBackGrounder(id, count) {
	BackGrounder.call(this, id, "tide");
  this.count = count;
}
SeaBackGrounder.prototype = new BackGrounder();
SeaBackGrounder.prototype.initialize = function() {
	BackGrounder.prototype.initialize.call(this);

	for(var n=0; n < this.count; n++)
    this.createUnit();
}
SeaBackGrounder.prototype.resize = function(w, h) {
	this.earth.resize(w, h);
}
SeaBackGrounder.prototype.onCreateUnit = function() {
  return new Tide(this);
}
SeaBackGrounder.prototype.onReleaseUnit = function(unit) {
	BackGrounder.prototype.onReleaseUnit.call(this, unit);

	this.createUnit();
}

var seaBg = new SeaBackGrounder("main-bg-sea", 10);
window.addEventListener('load', function(event) {
	seaBg.initialize();
});