#set terminal epslatex  mono
#set output "fig-betatime.eps" 

b_03_pt_ref = -5.40
b_03_car_ref = -27.7
b_03_pt_cl = 1.85
b_03_car_cl = 0.518

b_04_pt_ref = -3.15
b_04_car_ref = -13.5
b_04_pt_cl = 0.662
b_04_car_cl = -0.401

b_03_pt(x) = b_03_pt_ref * exp(b_03_pt_cl * x)
b_03_car(x) = b_03_car_ref * exp(b_03_car_cl * x)

b_04_pt(x) = b_04_pt_ref * exp(b_04_pt_cl * x)
b_04_car(x) = b_04_car_ref * exp(b_04_car_cl * x)

set xrange[-2:2] 
plot b_04_pt(x),b_04_car(x)