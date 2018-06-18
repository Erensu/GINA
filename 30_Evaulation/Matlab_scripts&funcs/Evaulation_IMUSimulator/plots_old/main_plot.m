% K�pes beolvasni egy trajectoria f�jlt. ok
% Ezt ki kell tudni plotolni.
% LLH egy plot de k�l�n subploton a koordin�t�k. ECEF-fel ugyanez. 

% F�ggelten att�l, hogy a traject�ria f�jlban, hogy volt megadva, az ECEF �s
% LLH kel.
% Sz�val tudni kell �tv�ltani is. OK

clc;
clear all;
close all;
fileName = 'trajectory_corner.gina';
t_data = create_data(fileName);
clc;    

enu_plot(t_data); %Ez haszn�lja az xyz2enu()-t ami �tv�lt rad-ba...
trajectroy3d_plot(t_data,'llh');
trajectroy3d_plot(t_data,'ecef');
attitude_plot(t_data);
lat_long_plot(t_data);


%ned_plot(t_data);




