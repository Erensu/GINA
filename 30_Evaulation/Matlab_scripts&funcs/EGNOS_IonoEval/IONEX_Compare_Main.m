
%folder = fileparts(which(mfilename)); 
%addpath(genpath(folder));

% in_folder = 'C:\Users\lus2bp\Source\Repos\GINA\30_Evaulation\Runnables&Data\2018\comp';
% out_Folder = 'C:\Users\lus2bp\Source\Repos\GINA\30_Evaulation\Runnables&Data\2018\comp_results';
% ext = '18I';

if ~exist(out_Folder, 'dir')
  mkdir(out_Folder);
end

out_dataFolder = fullfile(out_Folder,'data');
out_picsFolder_Base = fullfile(out_Folder,'maps');

if ~exist(out_dataFolder, 'dir')
  mkdir(out_dataFolder);
end
if ~exist(out_picsFolder_Base, 'dir')
  mkdir(out_picsFolder_Base);
end

cd(in_folder)
dinfo = dir(['*.',ext]);

for i = 1:length(dinfo)
    
    inFileName = dinfo(i).name;
    [pathstr, baseFileName, ext] = fileparts(inFileName);
    
    filewPath = fullfile(in_folder, inFileName);
    
    out_picsFolder = fullfile(out_picsFolder_Base,['Maps_', baseFileName]);
    
    if ~exist(out_picsFolder, 'dir')
        mkdir(out_picsFolder);
    end
    IONEX_Compare(filewPath, out_dataFolder, out_picsFolder);
end








clear all; close all; clc; 











