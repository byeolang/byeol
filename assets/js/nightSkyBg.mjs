import { animateNaviIcon } from './masthead-icon-animation.mjs'

var __star_dom_id_generator = 0;
function Star(new_x, new_y, new_w, new_h, new_life, new_owner, new_grade) {
	Unit.call(this, "star" + __star_dom_id_generator++, "/assets/images/star.png",
		new_x, new_y, new_w, new_h,
		new_life, new_owner, "opacity: 0; ");

	this.grade = new_grade;
}
Star.prototype = new Unit();
Star.prototype.getGrade = function() {
	return this.grade;
}
Star.prototype.initialize = function() {
	Unit.prototype.initialize.call(this);

	var total_rotation_amound = this.grade * 3.6;
	this.opacity = this.grade / 100;
	if(this.getJQObject() == undefined) return;

	TweenMax.to(this.getJQObject(), 1, {
		opacity: this.opacity,
		ease: Linear.easeNone
	});

	TweenMax.to(this.getJQObject(), this.life, {
		rotation: total_rotation_amound,
		ease: Linear.easeNone
	});
}


function FloatingStar(new_owner) {
	var grade = Math.random() * 100;
	var life = grade / 50;

	//	Big FloatingStar = ~ %3 of floating stars.
	if(grade > 97)
		grade *= 3;

	var	x = Math.random() * new_owner.getJQObject().width(),
		y = Math.random() * new_owner.getJQObject().height() * 0.6,
		w = grade * 0.3,
		h = w;

	Star.call(this, x, y, w, h, life, new_owner, grade);
}
FloatingStar.prototype = new Star();
FloatingStar.prototype.initialize = function() {
	Star.prototype.initialize.call(this);

	TweenMax.to(this.getJQObject(), 0.5, {
		delay: this.life-0.5,
		opacity: "0",
		width: "0px",
		height: "0px"
	});
}



function SilenceStar(new_owner) {
	var grade = Math.random() * 100;
	var life = grade / 20;

	var	x = Math.random() * new_owner.getJQObject().width(),
		y = Math.random() * new_owner.getJQObject().height() * 0.6,
		w = grade * 0.1,
		h = w;

	Star.call(this, x, y, w, h, life, new_owner, grade);
}
SilenceStar.prototype = new Star();
SilenceStar.prototype.initialize = function() {
	Star.prototype.initialize.call(this);
}


function ShootingStar(new_owner) {
	var grade = Math.random() * 100;
	var life = grade / 50;

	var	x = Math.random() * new_owner.getJQObject().width() * 0.5;
	var y = Math.random() * new_owner.getJQObject().height() * 0.2;
	var w = grade * 1.2;
	var h = w;

	Star.call(this, x, y, w, h, life, new_owner, grade);
}
ShootingStar.prototype = new Star();
ShootingStar.prototype.initialize = function() {
	Star.prototype.initialize.call(this);

	this.x2 = Math.random() * ($(window).width() - this.getX()) * this.grade / 300;
	TweenMax.to(this.getJQObject(), this.life, {
		delay: 0.2,
		left: (this.x2 + this.getX()) + "px",
		top: (this.x2 + this.getY()) + "px",
		ease: Power4.easeOut
	});
}

export function NightSkyBackGrounder(id, starCount) {
	BackGrounder.call(this, id, "star");
  this.starCount = starCount;
}
NightSkyBackGrounder.prototype = new BackGrounder();
NightSkyBackGrounder.prototype.initialize = function() {
	BackGrounder.prototype.initialize.call(this);


	for(var n=0; n < this.starCount; n++)
	{
		this.units[n] = new FloatingStar(this);
		this.units[n].initialize();
	}
}
NightSkyBackGrounder.prototype.resize = function(w, h) {
	this.earth.resize(w, h);
}
NightSkyBackGrounder.prototype.onCreateUnit = function() {
	var diceroll = Math.random() * 100;

	if(diceroll > 98.5)
		 return new ShootingStar(this);
	else if(diceroll > 90)
 		return new FloatingStar(this);
	else
	 	return new SilenceStar(this);
}
NightSkyBackGrounder.prototype.onReleaseUnit = function(unit) {
	BackGrounder.prototype.onReleaseUnit.call(this, unit);

	this.createUnit();
}

var nightSkyBg = new NightSkyBackGrounder("main-bg", 60);
window.addEventListener('resize', function(event) {
	updateWindow();
});

function updateWindow() {
	let vh = window.innerHeight * 0.01
	var vheader = document.getElementById('masthead').clientHeight;
	var vfooter = document.getElementById('footer').clientHeight;
	document.documentElement.style.setProperty("--vh", `${vh}px`);
	document.documentElement.style.setProperty("--vfooter", `${vfooter}px`);
	document.documentElement.style.setProperty("--vheader", `${vheader}px`);
}

window.addEventListener('load', function(event) {
	updateWindow();
	//nightSkyBg.initialize();
  setTimeout(animateNaviIcon, 3000);
});