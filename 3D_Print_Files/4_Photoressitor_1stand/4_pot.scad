base_d=35;
base_h=2;

wall_tk=2;
wall_h=20;
wall_n=4;

diode_r=2.5 ; //r=x^2+y^2
diode_d=5.6 ;

depth=.8 ;
difference(){
	union(){
		cylinder(d=base_d,h=base_h,$fn=60);
		for (i = [0:360/wall_n:360]) {
			color("orange")rotate([0,0,i])translate([-wall_tk/2,0,base_h])cube([wall_tk,base_d/2,wall_h]);
		}
	}
	union(){
		for (i = [0:360/wall_n:360]) {
			color("green")rotate([0,0,i])translate([wall_tk+diode_r,wall_tk+diode_r,base_h-depth])rotate([0,0,45])diode();
		}
	}
}


module diode(){
	cylinder(d=diode_d,h=5,$fn=60);
	translate([0,0,5])sphere(d=diode_d,h=5,$fn=60);
	translate([0,1,-5])cylinder(d=.5,h=5,$fn=60);
	translate([0,-1,-5])cylinder(d=.5,h=5,$fn=60);
}