% plotPLRequirements - This script generates plots to check protection
%                      level requirements
%
% Inputs:
%   outputPath - [string] outputfolder path including the current time and
%                         datasetname
%   ObjError - [Struct] structure which contains errors of different signals
%   ObjResults - [Struct] structure which contains results from requiremnt
%                         check function - evaluateSigmaRequirements.m
%   ObjConf - [Struct] Contains booleans to evaluate or not
%   ObjReq - [Struct] - Contains limits for requirements
%   ObjMasks - [Struct] - Contains different masks e.g. iTrace normal state
%                         mask
%   strResult - [string] Result of requirement check- Passed/Failed
%   fh - activate GUI
%
% Outputs:
%           -
% Created by -
% Date: -
% Date: 17.09.2018

function Results = plotPLRequirements(outputPath,ObjResults,ObjError, ObjDUT, ObjConf, ObjReq, ObjMasks, strResult,ObjRef, fh)

if nargin >9
    env='GUI';
else
    env='script';
end

% Reference quality flags
if isfield(ObjRef,'iTrace') && isfield(ObjRef.iTrace,'QualityFlag')
    QualityFlag = ObjRef.iTrace.QualityFlag;
else
    QualityFlag.Data = nan(size(ObjDUT.MasterTime));
    QualityFlag.time = ObjDUT.MasterTime;
end

[color,timestampsIndex,legendText] = getiTraceQualityFlags(QualityFlag);

cprintf([0,0,1],'Creating plots for Protection Level Requirements\n');
i = 1;

mkdir(outputPath);

% drivenPathSegments = lib_ExtractDrivenPaths(ObjDUT.Fusion,'POI');

ObjMasks.Masks.mask_RunningInTime = ObjMasks.Masks.mask_RunningInTime';

pparam =setPlotParameters(outputPath);

ObjSpecificMask.Data = and(ObjMasks.Masks.mask_TTO,ObjMasks.Masks.UserMasks);
ObjSpecificMask.time = ObjDUT.Fusion.Pos_E2POI_E_x_m.time;
ObjNormalState = ObjMasks.Masks.mask_transitions_met;
mask =and(ObjNormalState,ObjSpecificMask.Data);

% ******************************** Alert Limit 50 m Longitudinal ********************************
if (isfield(ObjConf,'PL_E_POI_Bd_x_m_LOK_Soft_247') == 1) && (ObjConf.PL_E_POI_Bd_x_m_LOK_Soft_247 == 1)

    plTime = ObjDUT.Fusion.PL_Longitudinal50_m.time;
    plData = ObjDUT.Fusion.PL_Longitudinal50_m.Data;
    errorTime = ObjError.PosError_E2POI_B_x_m.time;
    errorData = ObjError.PosError_E2POI_B_x_m.Data;
    stdTime = ObjDUT.Fusion.CovPos_E2POI_B_x_m.time;
    stdData = ObjDUT.Fusion.CovPos_E2POI_B_x_m.Data.^.5;
    velocity=(ObjDUT.Fusion.Vel_E2POI_N_e_m_s.Data.^2 + ObjDUT.Fusion.Vel_E2POI_N_n_m_s.Data.^2 + ObjDUT.Fusion.Vel_E2POI_N_u_m_s.Data.^2).^.5;
    strRequirementName = 'PL Longitudinal LOK Soft 247 (50m)';


    createStanfordDiagram(abs(errorData(mask)),plData(mask),25,velocity(mask),ObjReq.PL_E_POI_Bd_x_m_LOK_Soft_247.AlertLimit_m...
                           ,strRequirementName,'velocity [m/s] ',strcat(outputPath,'/Stanford_Diagram_velocity_','PL Longitudinal LOK Soft 247'),pparam);

    errorDataUnmasked = errorData;
    plDataUnmasked = plData;

    errorData(~mask) = nan;
    plData(~mask) = nan;
    listObjPloting = generatePLplotObject(errorData,errorDataUnmasked,errorTime,stdData,stdTime,plTime,plData, plDataUnmasked, ObjMasks, ObjReq.PL_E_POI_Bd_x_m_LOK_Soft_247,ObjResults(1),strResult(1,:),strRequirementName);

    % Add quality flag
    listObjPloting.backgroundArea.timestamps = timestampsIndex;
    listObjPloting.backgroundArea.color = color;
    listObjPloting.LegendColorText = legendText;

    % Define title for the figure
    listObjPloting.ytitles = {'Longitudinal error [m]'};

    i = i+1;

    if strcmp(env,'script')
        applyPlotParameters(listObjPloting,pparam);
    else
        VMPlots_GUI(listObjPloting,pparam,fh);
    end

end

% % ******************************** Alert Limit 3.2 Longitudinal ********************************

if (isfield(ObjConf,'PL_E_POI_Bd_x_m_LOK_Soft_150') == 1) && (ObjConf.PL_E_POI_Bd_x_m_LOK_Soft_150 == 1)

    plTime = ObjDUT.Fusion.PL_Longitudinal20_m.time;
    plData = ObjDUT.Fusion.PL_Longitudinal20_m.Data;
    errorTime = ObjError.PosError_E2POI_B_x_m.time;
    errorData = ObjError.PosError_E2POI_B_x_m.Data;
    stdTime = ObjDUT.Fusion.CovPos_E2POI_B_x_m.time;
    stdData = ObjDUT.Fusion.CovPos_E2POI_B_x_m.Data.^.5;
    strRequirementName = 'PL Longitudinal LOK Soft 150 (20m)';
    velocity=(ObjDUT.Fusion.Vel_E2POI_N_e_m_s.Data.^2 + ObjDUT.Fusion.Vel_E2POI_N_n_m_s.Data.^2 + ObjDUT.Fusion.Vel_E2POI_N_u_m_s.Data.^2).^.5;


	createStanfordDiagram(abs(errorData(mask)),plData(mask),25,velocity(mask),ObjReq.PL_E_POI_Bd_x_m_LOK_Soft_150.AlertLimit_m...
                           ,strRequirementName,'velocity [m/s] ',strcat(outputPath,'/Stanford_Diagram_velocity_','PL Longitudinal LOK Soft 150 (20m)'),pparam);

    errorDataUnmasked = errorData;
    plDataUnmasked = plData;

    errorData(~mask) = nan;
    plData(~mask) = nan;
    listObjPloting = generatePLplotObject(errorData,errorDataUnmasked,errorTime,stdData,stdTime,plTime,plData, plDataUnmasked, ObjMasks,  ObjReq.PL_E_POI_Bd_x_m_LOK_Soft_150,ObjResults(2),strResult(2,:),strRequirementName);

    % Add quality flag
    listObjPloting.backgroundArea.timestamps = timestampsIndex;
    listObjPloting.backgroundArea.color = color;
    listObjPloting.LegendColorText = legendText;

    % Define title for the figure
    listObjPloting.ytitles = {'Longitudinal error [m]'};

    i = i+1;

    if strcmp(env,'script')
        applyPlotParameters(listObjPloting,pparam);
    else
        VMPlots_GUI(listObjPloting,pparam,fh);
    end

end

% % ******************************** Alert Limit 4.1 Lateral ********************************
if((isfield(ObjConf,'PL_E_POI_Bd_y_m_LOK_Soft_149') == 1) && (ObjConf.PL_E_POI_Bd_y_m_LOK_Soft_149 == 1))

    plTime = ObjDUT.Fusion.PL_Lateral12_m.time;
    plData = ObjDUT.Fusion.PL_Lateral12_m.Data;
    errorTime = ObjError.PosError_E2POI_B_y_m.time;
    errorData = ObjError.PosError_E2POI_B_y_m.Data;
    stdTime = ObjDUT.Fusion.CovPos_E2POI_B_y_m.time;
    stdData = ObjDUT.Fusion.CovPos_E2POI_B_y_m.Data.^.5;
    strRequirementName = 'PL Lateral LOK Soft 149 (12m)';
    velocity=(ObjDUT.Fusion.Vel_E2POI_N_e_m_s.Data.^2 + ObjDUT.Fusion.Vel_E2POI_N_n_m_s.Data.^2 + ObjDUT.Fusion.Vel_E2POI_N_u_m_s.Data.^2).^.5;

    createStanfordDiagram(abs(errorData(mask)),plData(mask),25,velocity(mask),ObjReq.PL_E_POI_Bd_y_m_LOK_Soft_149.AlertLimit_m...
                         ,strRequirementName,'velocity [m/s] ',strcat(outputPath,'/Stanford_Diagram_velocity_','PL Lateral LOK Soft 149'),pparam);

    errorDataUnmasked = errorData;
    plDataUnmasked = plData;

    errorData(~mask) = nan;
    plData(~mask) = nan;
    listObjPloting = generatePLplotObject(errorData,errorDataUnmasked,errorTime,stdData,stdTime,plTime,plData, plDataUnmasked, ObjMasks,  ObjReq.PL_E_POI_Bd_y_m_LOK_Soft_149,ObjResults(3),strResult(3,:),strRequirementName);

    % Add quality flag
    listObjPloting.backgroundArea.timestamps = timestampsIndex;
    listObjPloting.backgroundArea.color = color;
    listObjPloting.LegendColorText = legendText;

    % Define title for the figure
    listObjPloting.ytitles = {'Lateral error [m]'};

    i = i+1;

    if strcmp(env,'script')
        applyPlotParameters(listObjPloting,pparam);
    else
        VMPlots_GUI(listObjPloting,pparam,fh);
    end

end

if (i == 1)
    Results = [];
end

close all;

end

