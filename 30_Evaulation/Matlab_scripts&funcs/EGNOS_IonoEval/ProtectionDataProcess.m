function ProtectionDataProcess(fileInwPath, fileOutwPath)

   
    [filepath,name,ext] = fileparts(fileInwPath);
    [filepath_out,name_out,ext_out] = fileparts(fileOutwPath);
    outFilewPath = fullfile(filepath,[name_out,'.txt']);

    [fin, errormsg] = fopen(fileInwPath, 'r');

    outPut_PL_Data = fullfile(fullfile(filepath_out), [name_out,'.txt']);

    fileID = fopen(outPut_PL_Data,'w');

    while ~feof(fin)

        tline = fgetl(fin);
        if strfind(tline, 'START OF DATA')

            [HPL, VPL, HP_error, VP_error, HAL, VAL, NumberOfSat] = processPLBlock(fin);
            fprintf(fileID,'%6.3f %6.3f %6.3f %6.3f %6.3f %6.3f %d \n',HPL, VPL, HP_error, VP_error, HAL, VAL, NumberOfSat);
        end
    end

    fclose(fileID);
    fclose(fin);

    %% Create Stanford diagramm
    if exist(outPut_PL_Data, 'file') ~= 2
        error('%s file open error\n', outPut_PL_Data);
    end

    Stanford_Plot(outPut_PL_Data);



end