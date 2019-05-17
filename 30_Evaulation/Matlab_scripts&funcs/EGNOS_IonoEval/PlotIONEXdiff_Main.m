

%% Start of script
%folder = fileparts(which(mfilename)); 
%addpath(genpath(folder));

 in_folder = 'C:\Users\lus2bp\Source\Repos\GINA\30_Evaulation\Runnables&Data\2018\comp_results_20190515_NightRun\data';
 out_folder_Base = 'C:\Users\lus2bp\Source\Repos\GINA\30_Evaulation\Runnables&Data\2018\comp_results_20190515_NightRun\data\Stats';


if ~exist(out_folder_Base, 'dir')
  mkdir(out_folder_Base);
end
% 
fitGauss = 1;
IntervalProbability = 0.0027;
binNumberOfAllHist = 50;
OnlyAllDataHist = 1;

TECbool = 1;
RMSbool = 1;
MLHbool = 1;
CH2bool = 1;

%% TEC
if(TECbool == 1)
    
    cd(in_folder)
    dinfo = dir('*.ionTEC');

    extraPlotSubFolder = 'TEC';
    PrefixforGrids = 'TEC_';
    PrefixAll = 'All_Day_Data_TEC';
    
    PostFixAll = '';
 
    for i = 1:length(dinfo)

        inPutFileName = dinfo(i).name;
        
        [pathstr, baseFileName, ext] = fileparts(inPutFileName);
        
        baseFileNameCut = strrep(baseFileName,'_TEC','');
        baseFileNameCut = strrep(baseFileNameCut,'PlotData_Comp_','');
        out_folder = fullfile(out_folder_Base, baseFileNameCut);
        
        if ~exist(out_folder, 'dir')
          mkdir(out_folder);
        end
        
        inPutFile = fullfile(in_folder,inPutFileName);
        sub_folder_out = fullfile(out_folder, extraPlotSubFolder);
        
        if ~exist(sub_folder_out, 'dir')
          mkdir(sub_folder_out);
        end
        
        inPutFile
        sub_folder_out
        
        PlotIONEXdiff(  inPutFile,              ...
                        sub_folder_out,         ...
                        PrefixforGrids,         ...
                        PrefixAll,              ...
                        PostFixAll,             ...
                        OnlyAllDataHist,        ...
                        fitGauss,               ...
                        IntervalProbability,	...
                        binNumberOfAllHist);

    end
 
end

%% RMS
if(RMSbool == 1)
    
    cd(in_folder)
    dinfo = dir('*.ionRMS');

    extraPlotSubFolder = 'RMS';
    PrefixforGrids = 'RMS_';
    PrefixAll = 'All_Day_Data_RMS';
    
    PostFixAll = '';
 
    for i = 1:length(dinfo)

       inPutFileName = dinfo(i).name;
        
        [pathstr, baseFileName, ext] = fileparts(inPutFileName);
        
        baseFileNameCut = strrep(baseFileName,'_RMS','');
        baseFileNameCut = strrep(baseFileNameCut,'PlotData_Comp_','');
        out_folder = fullfile(out_folder_Base, baseFileNameCut);
        
        if ~exist(out_folder, 'dir')
          mkdir(out_folder);
        end
        
        inPutFile = fullfile(in_folder,inPutFileName);
        sub_folder_out = fullfile(out_folder, extraPlotSubFolder);
        
        if ~exist(sub_folder_out, 'dir')
          mkdir(sub_folder_out);
        end
        
        inPutFile
        sub_folder_out
        
        PlotIONEXdiff(  inPutFile,              ...
                        sub_folder_out,     ...
                        PrefixforGrids,         ...
                        PrefixAll,              ...
                        PostFixAll,             ...
                        OnlyAllDataHist,        ...
                        fitGauss,               ...
                        IntervalProbability,	...
                        binNumberOfAllHist);

    end
 
end

%% MLH
if(MLHbool == 1)
    
    cd(in_folder)
    dinfo = dir('*.ionMLH');

    extraPlotSubFolder = 'MLH';
    PrefixforGrids = 'MLH_';
    PrefixAll = 'All_Day_Data_MLH';
    
    PostFixAll = '';
 
    for i = 1:length(dinfo)

        inPutFileName = dinfo(i).name;
        
        [pathstr, baseFileName, ext] = fileparts(inPutFileName);
        
        baseFileNameCut = strrep(baseFileName,'_MLH','');
        baseFileNameCut = strrep(baseFileNameCut,'PlotData_Comp_','');
        out_folder = fullfile(out_folder_Base, baseFileNameCut);
        
        if ~exist(out_folder, 'dir')
          mkdir(out_folder);
        end
        
        inPutFile = fullfile(in_folder,inPutFileName);
        sub_folder_out = fullfile(out_folder, extraPlotSubFolder);
        
        if ~exist(sub_folder_out, 'dir')
          mkdir(sub_folder_out);
        end
        
        inPutFile
        sub_folder_out
        
        PlotIONEXdiff(  inPutFile,              ...
                        sub_folder_out,         ...
                        PrefixforGrids,         ...
                        PrefixAll,              ...
                        PostFixAll,             ...
                        OnlyAllDataHist,        ...
                        fitGauss,               ...
                        IntervalProbability,	...
                        binNumberOfAllHist);

    end
 
end

%% CH2
if(CH2bool == 1)
    
    cd(in_folder)
    dinfo = dir('*.ionCH2');

    extraPlotSubFolder = 'CH2';
    PrefixforGrids = 'CH2_';
    PrefixAll = 'All_Day_Data_CH2';
    
    PostFixAll = '';
 
    for i = 1:length(dinfo)

        inPutFileName = dinfo(i).name;
        
        [pathstr, baseFileName, ext] = fileparts(inPutFileName);
        
        baseFileNameCut = strrep(baseFileName,'_CH2','');
        baseFileNameCut = strrep(baseFileNameCut,'PlotData_Comp_','');
        out_folder = fullfile(out_folder_Base, baseFileNameCut);
        
        if ~exist(out_folder, 'dir')
          mkdir(out_folder);
        end
        
        inPutFile = fullfile(in_folder,inPutFileName);
        sub_folder_out = fullfile(out_folder, extraPlotSubFolder);
        
        if ~exist(sub_folder_out, 'dir')
          mkdir(sub_folder_out);
        end
        
        inPutFile
        sub_folder_out
        
        PlotIONEXdiff(  inPutFile,              ...
                        sub_folder_out,     ...
                        PrefixforGrids,         ...
                        PrefixAll,              ...
                        PostFixAll,             ...
                        OnlyAllDataHist,        ...
                        fitGauss,               ...
                        IntervalProbability,	...
                        binNumberOfAllHist);

    end
 
end

%% End of script
clear all; close all; clc; 

% folder = fileparts(which(mfilename)); 
% addpath(genpath(folder));
% 
% plot_file = '\DebugOutputs\PlotData_Diff_CODG190-y2019_d019_136_TEC.ion';  %just for testing
% %sorted_plot_file = 'Sorted_PlotData_Diff_CODG190-y2019_d019_136_TEC.ion';
% extraPlotSubFolder = 'Plots\TEC';
% PrefixforGrids = 'TEC_';
% PrefixAll = 'All_Day_Data_TEC';
% PostFixAll = '';
% OnlyAllDataHist = 1;
% 
% fitGauss = 1;
% IntervalProbability = 0.0027;
% binNumberOfAllHist = 50;
% 
% PlotIONEXdiff(plot_file, extraPlotSubFolder, PrefixforGrids, PrefixAll, PostFixAll, OnlyAllDataHist, fitGauss, IntervalProbability, binNumberOfAllHist);

clear all;
close all; clc; 