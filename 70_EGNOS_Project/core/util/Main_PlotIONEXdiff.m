
clear all; close all; clc; 
folder = fileparts(which(mfilename)); 
addpath(genpath(folder));

plot_file = '\DebugOutputs\PlotData_Diff_CODG190-y2019_d019_136_MLH.ion';  %just for testing
sorted_plot_file = 'Sorted_PlotData_Diff_CODG190-y2019_d019_136_MLH.ion';
extraPlotSubFolder = 'Plots\MLH';
PrefixforGrids = 'MLH_';
PrefixAll = 'All_Day_Data_MLH';
PostFixAll = '';
OnlyAllDataHist = 1;

gridBinNumber = 10;
AllDataBinNumber = 200;

PlotIONEXdiff(plot_file, sorted_plot_file, extraPlotSubFolder, PrefixforGrids, PrefixAll, PostFixAll, OnlyAllDataHist, gridBinNumber, AllDataBinNumber);