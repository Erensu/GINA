function [HPL, VPL, HP_error, VP_error, HAL, VAL, NumberOfSat] = processPLBlock(fin)

    HPL = 0;
    VPL = 0;
    HP_error = 0;
    VP_error = 0;
    HAL = 0;
    VAL = 0;
    NumberOfSat = 0;
    
    while ~feof(fin)

        tline = fgetl(fin);
        if strfind(tline, 'END OF DATA')

            if isempty(HAL)
                HAL = 0;
            end

            if isempty(VAL)
                VAL = 0;
            end
            return;
        end

        preFixStr = 'START OF USED SAT';
        if strfind(tline, preFixStr)
            NumberOfSat = 0;
        	postFixStr = 'END OF USED SAT';
            while ~feof(fin)
                tline = fgetl(fin); 
                if strfind(tline, postFixStr)
                    break;
                end
                NumberOfSat = NumberOfSat + 1;
            end
            
        end

        preFixStr = 'HPL';
        if strfind(tline, preFixStr)
            HPL = str2num(tline( (length(preFixStr)+1):length(tline)));
        end

        preFixStr = 'VPL';
        if strfind(tline, preFixStr)
            VPL = str2num(tline( (length(preFixStr)+1):length(tline)));
        end

        preFixStr = 'HP ERROR';
        if strfind(tline, preFixStr)
            HP_error = str2num(tline( (length(preFixStr)+1):length(tline)));
        end

        preFixStr = 'VP ERROR';
        if strfind(tline, preFixStr)
            VP_error = str2num(tline( (length(preFixStr)+1):length(tline)));
        end

        preFixStr = 'HORIZONTAL ALARM LIMIT';
        if strfind(tline, preFixStr)
            HAL = str2num(tline( (length(preFixStr)+1):length(tline)));
        end

        preFixStr = 'VERTICAL ALARM LIMIT';
        if strfind(tline, preFixStr)
            VAL = str2num(tline( (length(preFixStr)+1):length(tline)));
        end
    end
    
end