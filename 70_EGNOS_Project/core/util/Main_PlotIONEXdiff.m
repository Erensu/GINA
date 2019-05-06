
clear all; close all; clc; 
folder = fileparts(which(mfilename)); 
addpath(genpath(folder));

plot_file = '\DebugOutputs\PlotData_Diff_CODG190-y2019_d019_136_TEC.ion';  %just for testing
sorted_plot_file = 'Sorted_PlotData_Diff_CODG190-y2019_d019_136_TEC.ion';
extraPlotSubFolder = 'Plots\TEC';
PrefixforGrids = 'TEC_';
PrefixAll = 'All_Day_Data_TEC';
PostFixAll = '';
OnlyAllDataHist = 1;

fitGauss = 1;
IntervalProbability = 0.0027;

PlotIONEXdiff(plot_file, sorted_plot_file, extraPlotSubFolder, PrefixforGrids, PrefixAll, PostFixAll, OnlyAllDataHist, fitGauss, IntervalProbability);