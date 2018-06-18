% K�pes beolvasni egy trajectoria f�jlt. ok
% Ezt ki kell tudni plotolni.
% LLH egy plot de k�l�n subploton a koordin�t�k. ECEF-fel ugyanez. 

% F�ggelten att�l, hogy a traject�ria f�jlban, hogy volt megadva, az ECEF �s
% LLH kel.
% Sz�val tudni kell �tv�ltani is. OK

clc;
clear all;
close all;
%fileName = '../gina_files/example_trajectory_out_steady_state.gina';

fileName = '../gina_files/new_traj.gina';

%fileName3 = '../gina_files/example_trajectory_out_vx_1ms_yaw_1.51.gina';

%fileName = 'trajectory_corner.gina';
t_data = create_data(fileName);
clc;    


       
%[e,n,u]= xyz2enu(lat,lon,start_xyz(1), start_xyz(2), start_xyz(3));
enu_plot(t_data,'2D'); %Ez haszn�lja az xyz2enu()-t ami �tv�lt rad-ba...
enu_plot(t_data,'3D');
%enu2_plot(t_data,'2D');
%ned_plot(t_data);
trajectroy3d_plot(t_data,'llh');
trajectroy3d_plot(t_data,'ecef');
attitude_plot(t_data);
lat_long_plot(t_data);
% figure('name','geoshow')
% geoshow(t_data.lat,t_data.lon)
% S = shaperead('landareas','UseGeoCoords',true);
% geoshow([S.Lat], [S.Lon],'Color','black');
% 
% 
% figure
% 
% x = -3:0.5:3;
% y = -3:0.5:3;
% [X,Y] = meshgrid(x, y);
% Z = Y.^2 - X.^2;
% [U,V,W] = surfnorm(Z);
% figure
% quiver3(Z,U,V,W)
% view(-35,45)

