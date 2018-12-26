
ring_d=36;
spl=2; //spacer layer
tire_h=12;

ridge_w=1.2;
ridge_d=1.5;

ridge_n=30;


difference(){
	union(){
		color("black")
		difference(){
			cylinder(d=ring_d+spl,h=tire_h,$fn=120,center=true);
			cylinder(d=ring_d,h=tire_h+.1,$fn=120,center=true);
		}
		for (i = [0:360/ridge_n:360]) {
			color("orange")rotate([0,10,i])translate([0,ring_d/2+ridge_d/2,0])cube([ridge_w,ridge_d,tire_h*2],center=true);
		}
		rotate_extrude(convexity = 10, $fn = 100)translate([ring_d/2, 0, 0])circle(d = 1.2, $fn = 100);
	}
	union(){
		translate([0,0,-tire_h])cylinder(d=ring_d+spl*2,h=tire_h,$fn=120,center=true);
		translate([0,0,+tire_h])cylinder(d=ring_d+spl*2,h=tire_h,$fn=120,center=true);
	}
}
