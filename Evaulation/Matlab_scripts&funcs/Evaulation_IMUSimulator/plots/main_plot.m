% K�pes beolvasni egy trajectoria f�jlt. Ezt ki kell tudni plotolni.
% LLH egy plot de k�l�n subploton a koordin�t�k. ECEF-fel ugyanez. 

% F�ggelten att�l, hogy a traject�ria f�jlban, hogy volt megadva, az ECEF �s
% LLH kel.
% Sz�val tudni kell �tv�ltani is. OK

clc;
clear all;
fileName = 'imu.pina'

h_data= read_header(fileName);
data = read_data(fileName);

disp(h_data)
disp(data)

%t_data is a struct of trajecrtory
t_data_ecef = make_trajectory_format(data,'ECEF');

%ECEF->LLH ; LLH->ECF 
t_data_llh = change_trajectory_format(t_data_ecef);
t_data_ecef_new = change_trajectory_format(t_data_llh);

disp('Az �tv�lt�s maxim�lis abszol�t n�gyzetes hib�ja:')
x_max_abs_qad = max(abs(t_data_ecef_new.x.^2 - t_data_ecef.x.^2));
y_max_abs_qad = max(abs(t_data_ecef_new.y.^2 - t_data_ecef.y.^2));
z_max_abs_qad = max(abs(t_data_ecef_new.z.^2 - t_data_ecef.z.^2));
disp('x,y,z:')
disp( [x_max_abs_qad x_max_abs_qad z_max_abs_qad])

lat = [45,45,45,45,45,45,45,45,45,50,55,60,65,70,80,85,90]
lon = [10,20,30,40,50,60,70,80,90,90,90,90,90,90,90,90,90]
h =  [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]
lon = degtorad(lon)
lat = degtorad(lat)



 geoshow(lat,lon,'DisplayType','line')


 kmlwriteline('traj.kml',lat,lon,h)



% Set up the map axes
figure
axesm('globe','geoid',wgs84Ellipsoid)
% % Use topo data to create a background
load topo
 meshm(topo, topolegend, size(topo)); 
 demcmap(topo);
% Plot your track
plot3m(lat,lon,h,'r-')


