function onResize() {
	var x = Math.max(528,document.body.clientWidth - 349);
	document.getElementById("main").style.backgroundPosition = x+"px 0px";
}

function start() {
	onResize();
	window.onresize = onResize;
}
