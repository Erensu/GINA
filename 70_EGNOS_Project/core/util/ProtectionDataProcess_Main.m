
clear all; close all; clc; 
folder = fileparts(which(mfilename)); 
addpath(genpath(folder));

pl_file = '\DebugFiles\PL_Data\Pos_lat47.464085lon19.154686h100_deltaT3600_el0_Tar_y2019_d019_136_Ref_CODG0190_brdc0190.pl';  %just for testing
sorted_pl_file = 'Sorted_Pos_lat47.464085lon19.154686h100_deltaT3600_el0_Tar_y2019_d019_136_Ref_CODG0190_brdc0190.pl';
extraPlotSubFolder = 'Plots';

filewPath = which(pl_file);
if exist(filewPath, 'file') ~= 2
   	error('%s file open error\n', filewPath);
end

[filepath,name,ext] = fileparts(filewPath);
[~,name_out,ext_out] = fileparts(sorted_pl_file);
outFilewPath = fullfile(filepath,[name_out,'.txt']);

[fin, errormsg] = fopen(filewPath, 'r');

fileID = fopen(fullfile(fullfile(filepath,extraPlotSubFolder), [name_out,'.txt']),'w');
   
while ~feof(fin)
    
    tline = fgetl(fin);
    if strfind(tline, 'START OF DATA')
        
        [HPL, VPL, HP_error, VP_error, HAL, VAL, NumberOfSat] = processPLBlock(fin);
     	fprintf(fileID,'%6.3f %6.3f %6.3f %6.3f %6.3f %6.3f %d \n',HPL, VPL, HP_error, VP_error, HAL, VAL, NumberOfSat);
    end
end

fclose(fileID);
fclose(fin);




