
folder = fileparts(which(mfilename)); 
addpath(genpath(folder));

% pl_folder = 'C:\Users\lus2bp\Source\Repos\GINA\70_EGNOS_Project\core\files\DebugFiles\PL_Data';
% pl_folder_out = 'C:\Users\lus2bp\Source\Repos\GINA\70_EGNOS_Project\core\files\DebugFiles\PL_Data\Results';
pl_folder
pl_folder_out
if ~exist(pl_folder_out, 'dir')
  mkdir(pl_folder_out);
end

cd(pl_folder)
dinfo = dir('*.pl');
for i = 1:length(dinfo)
    outPutFileName = dinfo(i).name;
    [pathstr, OutPLFileName, ext] = fileparts(outPutFileName);
    pls_file = fullfile(pl_folder_out, [OutPLFileName,'.pls']);
    ProtectionDataProcess(dinfo(i).name, pls_file);
    
end


clear all;
close all; clc; 