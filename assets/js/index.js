function Obj(new_jq_object, new_dom_id) {
	this.jq_object = new_jq_object;
	
	if(new_dom_id == undefined)
		new_dom_id = "";
	this.dom_id = new_dom_id;
}
Obj.prototype.setJQObject = function(new_obj) {
	this.jq_object = new_obj;
	if(	new_obj == undefined	||
		new_obj[0] == undefined	)
		return;
		
	this.jq_object.owner = new_obj[0].owner = this;
}
Obj.prototype.getJQObject = function() {
	return this.jq_object;
}
Obj.prototype.setID = function(new_id) {
	this.dom_id = new_id;
}
Obj.prototype.getID = function() {
	return this.dom_id;
}
Obj.prototype.release = function() {
	if(this.jq_object != undefined)
		this.jq_object.detach();
	this.jq_object = undefined; // JS has GC
}
Obj.prototype.purifyDomID = function(string) {
	if(string == undefined)
		return;
		
	return string.replace(/[\/?<>\\|:.,!@#$%^&*\(\)=+ ]/gi, "_");
}

function Unit(new_dom_id, new_image_path, 
    new_x, new_y, new_w, new_h, 
    new_life, new_owner, additional_style) {
    Obj.call(this, undefined, new_dom_id);

    this.path = new_image_path;
    this.x = new_x;
    this.y = new_y;
    this.width = new_w;
    this.height = new_h;
    this.life = new_life;
    this.owner = new_owner;
    if(additional_style == undefined)
        additional_style = "";
    this.style = additional_style;
    this._life_animation = undefined;
}
Unit.prototype = new Obj();
Unit.prototype.getLife = function() { return this.life; }
Unit.prototype.setLife = function(new_life) { this.life = new_life; }
Unit.prototype.getIndex = function() { return this.index; }
Unit.prototype.setIndex = function(new_index) { this.index = new_index; }
Unit.prototype.getPath = function() { return this.path; }
Unit.prototype.getX = function() { return this.x; }
Unit.prototype.getY = function() { return this.y; }
Unit.prototype.getWidth = function() { return this.width; }
Unit.prototype.getHeight = function() { return this.height; }
Unit.prototype.getStyle = function() { return this.style; }
Unit.prototype.getOwnedBackGrounder = function() { return this.owner; }
Unit.prototype.initialize = function() {
    var owner = this.getOwnedBackGrounder().getJQObject();
    if(owner == undefined)
        return;

    var style_attr =    "'position:fixed; z-index:0 " +
                        "; top: " + this.getY() +
                        "px; left: " + this.getX() +
                        "px; width: " + this.getWidth() +
                        "px; height: " + this.getHeight() + 
                        "px; " + this.getStyle() + "'";

    var tag =   "<img id='" + this.getID() + "' src='" + this.getPath() + "' style=" + style_attr +
                "/>"
    owner.append(tag);
    this.setJQObject($("img#" + this.getID()));

    if(this.life >= 0)
    {
        this._life_animation = new TweenMax(this.getJQObject(), 1, {
            delay: this.life,
            opacity: "0",
            onCompleteScope: this,
            onComplete: function() {
                var bg = this.getOwnedBackGrounder();
                if( bg == undefined         ||
                    bg.units == undefined  )
                    return;
                
                this.release();
            }
        });
        this._life_animation.play();
    }
}
Unit.prototype.release = function() {
    if(this.life >= 0)
        this.getOwnedBackGrounder().releaseUnit(this);

    Obj.prototype.release.call(this);
}

function BackGrounder(name) {
	Obj.call(this);
	this.name = name;
	this.is_initialized = false;
}
BackGrounder.prototype = new Obj();
BackGrounder.prototype.getName = function() { return this.name; }
BackGrounder.prototype.isInitialized = function() { return this.is_initialized; }
BackGrounder.prototype.initialize = function() {
	//	We don't need to release 'this':
	//		it requires 0.8sec at least to be released. Release function add some sort of timer for after 0.8sec.
	//		So even if it defines new instance of div which will be used to draw something,
	//		after it passed 0.8sec, that new div will be going to disappear because of that timer`
	//
	//	this.release();
	
	this.setID("main-bg");
	this.setJQObject($("div#" + this.getID()));

	this.units = [];
	this.is_initialized = true;
}
BackGrounder.prototype.release = function() {
	//	pre:
	//		supering:
	Obj.prototype.release.call(this);	
	//		type-checking:
	if( ! this.isInitialized()) return;
	var jqobj = this.getJQObject();
	if(jqobj == undefined) return;

	//	main:
	var height = jqobj.height();

	TweenMax.to(jqobj, 0.4, {
		top: height + "px", 
		onCompleteScope: this,
		onComplete: function() {
			for(var e in this.units)
			{
				this.units[e].release();
				this.units[e] = undefined;
			}
			this.units = undefined;
			//	don't need to remove 'this':
			//		The new BackGrounder which try to initialize will delete existing div.dyn_obj.
		}, 
		ease:Power4.easeOut
	});
	this.is_initialized = false;
}
BackGrounder.prototype.resize = function(w, h) {
	TweenMax.to(this.getJQObject(), 1, {
        ease: Power3.easeOut,
		width: w + "px"
	});
}
BackGrounder.prototype.releaseUnit = function(unit) {
	//	pre:
	//		parameter-check:
	if( ! this.isInitialized()) return;
	if(unit == undefined) return;
	//			is this my child?:
	if(this != unit.getOwnedBackGrounder()) return;

	return this.onReleaseUnit(unit);
}
BackGrounder.prototype.onReleaseUnit = function(unit) {		
	if(unit == undefined) return;

	this.units.splice(unit.getIndex());
}
BackGrounder.prototype.createUnit = function() {
	if(this.stop) return;

	var new_one = this.onCreateUnit();
	if(new_one == undefined) return;

	new_one.initialize();
	new_one.setIndex(this.units.length);
	
	this.units.push(new_one);
}

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

	var	x = Math.random() * $(window).width(),
		y = Math.random() * $(window).height() * 0.6,
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
	
	var	x = Math.random() * $(window).width(),
		y = Math.random() * $(window).height() * 0.6,
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

	var	x = Math.random() * $(window).width() * 0.5
		y = Math.random() * $(window).height() * 0.2,
		w = grade * 1.2,
		h = w;

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

function SkyLineBackGrounder(name) {
	BackGrounder.call(this, name);
}
SkyLineBackGrounder.prototype = new BackGrounder();
SkyLineBackGrounder.prototype.initialize = function() {
	BackGrounder.prototype.initialize.call(this);


	var star_count = 60;
	for(var n=0; n < star_count; n++)
	{
		this.units[n] = new FloatingStar(this);
		this.units[n].initialize();
	}
}
SkyLineBackGrounder.prototype.resize = function(w, h) {
	this.earth.resize(w, h);
}
SkyLineBackGrounder.prototype.onCreateUnit = function() {
	var diceroll = Math.random() * 100;

	if(diceroll > 98.5)
		 return new ShootingStar(this);
	else if(diceroll > 90)
 		return new FloatingStar(this);
	else
	 	return new SilenceStar(this);
}
SkyLineBackGrounder.prototype.onReleaseUnit = function(unit) {	
	BackGrounder.prototype.onReleaseUnit.call(this, unit);

	this.createUnit();
}

var bg = new SkyLineBackGrounder();

window.onresize = function(event) {
	updateWindow();
}
function updateWindow() {
	let vh = window.innerHeight * 0.01
	var vheader = document.getElementById('masthead').clientHeight;
	var vfooter = document.getElementById('footer').clientHeight;
	document.documentElement.style.setProperty("--vh", `${vh}px`);
	document.documentElement.style.setProperty("--vfooter", `${vfooter}px`);
	document.documentElement.style.setProperty("--vheader", `${vheader}px`);
}
window.onload = function(event) {
	updateWindow();
	bg.initialize();
}