
%folder = fileparts(which(mfilename)); 
%addpath(genpath(folder));


pl_file = 'C:\Users\lus2bp\Source\Repos\GINA\70_EGNOS_Project\core\files\DebugFiles\PL_Data\Pos_lat46.453826lon25.582440h1801_dt30_el10_Tar_y2019_d019_136_brdc0190.pl';
sorted_pl_file = 'C:\Users\lus2bp\Source\Repos\GINA\70_EGNOS_Project\core\files\DebugFiles\PL_Data\Plots\Sorted_Pos_lat46.453826lon25.582440h1801_dt30_el10_Tar_y2019_d019_136_brdc0190.pl';

ProtectionDataProcess(pl_file, sorted_pl_file);

clear all;
close all; clc; 