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
    var owner = this.getOwnedBackGrounder();
    var ownerObj = owner.getJQObject();
    if(ownerObj == undefined)
        return;

    var style_attr = "'top: " + this.getY() +
                     "px; left: " + this.getX() +
                     "px; width: " + this.getWidth() +
                     "px; height: " + this.getHeight() +
                     "px; " + this.getStyle() + "'";

    var tag =   "<img id='" + this.getID() + "' class='" + owner.getClassName() +
      "' src='" + this.getPath() + "' style=" + style_attr + "/>"
    ownerObj.append(tag);
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

function BackGrounder(id, classname) {
	Obj.call(this);
	this.is_initialized = false;
  this.setID(id);
  this.className = classname;
}
BackGrounder.prototype = new Obj();
BackGrounder.prototype.isInitialized = function() { return this.is_initialized; }
BackGrounder.prototype.getClassName = function() { return this.className; }
BackGrounder.prototype.initialize = function() {
	//	We don't need to release 'this':
	//		it requires 0.8sec at least to be released. Release function add some sort of timer for after 0.8sec.
	//		So even if it defines new instance of div which will be used to draw something,
	//		after it passed 0.8sec, that new div will be going to disappear because of that timer`
	//
	//	this.release();

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