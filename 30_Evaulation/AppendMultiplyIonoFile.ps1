$PSCommandPath
$PS_Main_path = $PSCommandPath | Split-Path -Parent

$in_folder = 'C:\Users\lus2bp\Source\Repos\GINA\30_Evaulation\Runnables&Data\2018\comp_results_2\data';

$tecExt = 'ionTEC'
$rmsExt = 'ionRMS'
$mlhExt = 'ionMLH'
$ch2Ext = 'ionCH2'

$IonTECFiles = Get-ChildItem $in_folder  -Filter *.$tecExt
$IonRMSFiles = Get-ChildItem $in_folder  -Filter *.$rmsExt
$IonMLHFiles = Get-ChildItem $in_folder  -Filter *.$mlhExt
$IonCH2Files = Get-ChildItem $in_folder  -Filter *.$ch2Ext

$codgMatchingSubWord = 'CODG'
$emsMatchingSubWord = '_d'

$IonTECTable =  @{}

$IonTECFiles | %{   

    $file = $_
    $fileName = $file.Name

    $matchString1 = $fileName -match "$codgMatchingSubWord"
    $matchString2 = $fileName -match "$emsMatchingSubWord"

    if ($matchString1){
        
        $day = [int]$fileName.SubString($fileName.IndexOf("$codgMatchingSubWord")+$codgMatchingSubWord.Length,3) 
    }
    elseif($matchString){
        
        $day = [int]$fileName.SubString($fileName.IndexOf("$emsMatchingSubWord")+$emsMatchingSubWord.Length,3)
 
    }

    if ($matchString1 -or $matchString2){

        $IonTECTable.add($day, $file)
    }

}

$IonRMSTable =  @{}

$IonRMSFiles | %{   

    $file = $_
    $fileName = $file.Name

    $matchString1 = $fileName -match "$codgMatchingSubWord"
    $matchString2 = $fileName -match "$emsMatchingSubWord"

    if ($matchString1){
        
        $day = [int]$fileName.SubString($fileName.IndexOf("$codgMatchingSubWord")+$codgMatchingSubWord.Length,3) 
    }
    elseif($matchString){
        
        $day = [int]$fileName.SubString($fileName.IndexOf("$emsMatchingSubWord")+$emsMatchingSubWord.Length,3)
 
    }

    if ($matchString1 -or $matchString2){
    
        $IonRMSTable.add($day, $file)
    }

}

$IonMLHTable =  @{}

$IonMLHFiles | %{   

    $file = $_
    $fileName = $file.Name

    $matchString1 = $fileName -match "$codgMatchingSubWord"
    $matchString2 = $fileName -match "$emsMatchingSubWord"

    if ($matchString1){
        
        $day = [int]$fileName.SubString($fileName.IndexOf("$codgMatchingSubWord")+$codgMatchingSubWord.Length,3) 
    }
    elseif($matchString){
        
        $day = [int]$fileName.SubString($fileName.IndexOf("$emsMatchingSubWord")+$emsMatchingSubWord.Length,3)
 
    }

    if ($matchString1 -or $matchString2){
    
        $IonMLHTable.add($day, $file)
    }

}

$IonCH2Table =  @{}

$IonCH2Files | %{   

    $file = $_
    $fileName = $file.Name

    $matchString1 = $fileName -match "$codgMatchingSubWord"
    $matchString2 = $fileName -match "$emsMatchingSubWord"

    if ($matchString1){
        
        $day = [int]$fileName.SubString($fileName.IndexOf("$codgMatchingSubWord")+$codgMatchingSubWord.Length,3) 
    }
    elseif($matchString){
        
        $day = [int]$fileName.SubString($fileName.IndexOf("$emsMatchingSubWord")+$emsMatchingSubWord.Length,3)
 
    }

    if ($matchString1 -or $matchString2){
    
        $IonCH2Table.add($day, $file)
    }

}


$concatenatedFolder = join-path $in_folder -ChildPath 'concatenatedFiles'
If(!(test-path $concatenatedFolder))
{
      New-Item -ItemType Directory -Force -Path $concatenatedFolder
}

# 0
#########################################

$dayStart = 1
$dayEnd = 365


    $OutPutNameTEC = join-path $concatenatedFolder -ChildPath $(-join("TEC_Day_",$dayStart, "_", $dayEnd,"." , $tecExt))
    if (Test-Path $OutPutNameTEC){   Remove-Item $OutPutNameTEC   }

    $OutPutNameRMS = join-path $concatenatedFolder -ChildPath $(-join("RMS_Day_",$dayStart, "_", $dayEnd,"." , $rmsExt))
    if (Test-Path $OutPutNameRMS){   Remove-Item $OutPutNameRMS   }

    $OutPutNameMLH = join-path $concatenatedFolder -ChildPath $(-join("MLH_Day_",$dayStart, "_", $dayEnd,"." , $mlhExt))
    if (Test-Path $OutPutNameMLH){   Remove-Item $OutPutNameMLH   }

    $OutPutNameCH2 = join-path $concatenatedFolder -ChildPath $(-join("CH2_Day_",$dayStart, "_", $dayEnd,"." , $ch2Ext))
    if (Test-Path $OutPutNameCH2){   Remove-Item $OutPutNameCH2   }
   
foreach($day in $($dayStart..$dayEnd)){

        ##
        $IonTEC = $IonTECTable[$day]
        $IonTECTable[$day].FullName
        
      
        if ($IonTECTable[$day].FullName){

            Get-Content $IonTECTable[$day].FullName | Add-Content $OutPutNameTEC 

        }

        ##
        $IonRMS = $IonRMSTable[$day]
        $IonRMSTable[$day].FullName
        
      
        if ($IonRMSTable[$day].FullName){

            Get-Content $IonRMSTable[$day].FullName | Add-Content $OutPutNameRMS 

        }

        ##
        $IonMLH = $IonMLHTable[$day]
        $IonMLHTable[$day].FullName
        
      
        if ($IonMLHTable[$day].FullName){

            Get-Content $IonMLHTable[$day].FullName | Add-Content $OutPutNameMLH 

        }

        
        ##
        $IonCH2 = $IonCH2Table[$day]
        $IonCH2Table[$day].FullName
        
      
        if ($IonCH2Table[$day].FullName){

            Get-Content $IonCH2Table[$day].FullName | Add-Content $OutPutNameCH2 

        }
        
        
}

# 1
#########################################

$dayStart = 1
$dayEnd = $dayStart + 30


    $OutPutNameTEC = join-path $concatenatedFolder -ChildPath $(-join("TEC_Day_",$dayStart, "_", $dayEnd,"." , $tecExt))
    if (Test-Path $OutPutNameTEC){   Remove-Item $OutPutNameTEC   }

    $OutPutNameRMS = join-path $concatenatedFolder -ChildPath $(-join("RMS_Day_",$dayStart, "_", $dayEnd,"." , $rmsExt))
    if (Test-Path $OutPutNameRMS){   Remove-Item $OutPutNameRMS   }

    $OutPutNameMLH = join-path $concatenatedFolder -ChildPath $(-join("MLH_Day_",$dayStart, "_", $dayEnd,"." , $mlhExt))
    if (Test-Path $OutPutNameMLH){   Remove-Item $OutPutNameMLH   }

    $OutPutNameCH2 = join-path $concatenatedFolder -ChildPath $(-join("CH2_Day_",$dayStart, "_", $dayEnd,"." , $ch2Ext))
    if (Test-Path $OutPutNameCH2){   Remove-Item $OutPutNameCH2   }
   
foreach($day in $($dayStart..$dayEnd)){

        ##
        $IonTEC = $IonTECTable[$day]
        $IonTECTable[$day].FullName
        
      
        if ($IonTECTable[$day].FullName){

            Get-Content $IonTECTable[$day].FullName | Add-Content $OutPutNameTEC 

        }

        ##
        $IonRMS = $IonRMSTable[$day]
        $IonRMSTable[$day].FullName
        
      
        if ($IonRMSTable[$day].FullName){

            Get-Content $IonRMSTable[$day].FullName | Add-Content $OutPutNameRMS 

        }

        ##
        $IonMLH = $IonMLHTable[$day]
        $IonMLHTable[$day].FullName
        
      
        if ($IonMLHTable[$day].FullName){

            Get-Content $IonMLHTable[$day].FullName | Add-Content $OutPutNameMLH 

        }

        
        ##
        $IonCH2 = $IonCH2Table[$day]
        $IonCH2Table[$day].FullName
        
      
        if ($IonCH2Table[$day].FullName){

            Get-Content $IonCH2Table[$day].FullName | Add-Content $OutPutNameCH2 

        }
        
        
}

# 2
#########################################

$dayStart = 31
$dayEnd = $dayStart + 29


    $OutPutNameTEC = join-path $concatenatedFolder -ChildPath $(-join("TEC_Day_",$dayStart, "_", $dayEnd,"." , $tecExt))
    if (Test-Path $OutPutNameTEC){   Remove-Item $OutPutNameTEC   }

    $OutPutNameRMS = join-path $concatenatedFolder -ChildPath $(-join("RMS_Day_",$dayStart, "_", $dayEnd,"." , $rmsExt))
    if (Test-Path $OutPutNameRMS){   Remove-Item $OutPutNameRMS   }

    $OutPutNameMLH = join-path $concatenatedFolder -ChildPath $(-join("MLH_Day_",$dayStart, "_", $dayEnd,"." , $mlhExt))
    if (Test-Path $OutPutNameMLH){   Remove-Item $OutPutNameMLH   }

    $OutPutNameCH2 = join-path $concatenatedFolder -ChildPath $(-join("CH2_Day_",$dayStart, "_", $dayEnd,"." , $ch2Ext))
    if (Test-Path $OutPutNameCH2){   Remove-Item $OutPutNameCH2   }
   
foreach($day in $($dayStart..$dayEnd)){

        ##
        $IonTEC = $IonTECTable[$day]
        $IonTECTable[$day].FullName
        
      
        if ($IonTECTable[$day].FullName){

            Get-Content $IonTECTable[$day].FullName | Add-Content $OutPutNameTEC 

        }

        ##
        $IonRMS = $IonRMSTable[$day]
        $IonRMSTable[$day].FullName
        
      
        if ($IonRMSTable[$day].FullName){

            Get-Content $IonRMSTable[$day].FullName | Add-Content $OutPutNameRMS 

        }

        ##
        $IonMLH = $IonMLHTable[$day]
        $IonMLHTable[$day].FullName
        
      
        if ($IonMLHTable[$day].FullName){

            Get-Content $IonMLHTable[$day].FullName | Add-Content $OutPutNameMLH 

        }

        
        ##
        $IonCH2 = $IonCH2Table[$day]
        $IonCH2Table[$day].FullName
        
      
        if ($IonCH2Table[$day].FullName){

            Get-Content $IonCH2Table[$day].FullName | Add-Content $OutPutNameCH2 

        }
        
        
}


# 3
#########################################

$dayStart = 61
$dayEnd = $dayStart + 29


    $OutPutNameTEC = join-path $concatenatedFolder -ChildPath $(-join("TEC_Day_",$dayStart, "_", $dayEnd,"." , $tecExt))
    if (Test-Path $OutPutNameTEC){   Remove-Item $OutPutNameTEC   }

    $OutPutNameRMS = join-path $concatenatedFolder -ChildPath $(-join("RMS_Day_",$dayStart, "_", $dayEnd,"." , $rmsExt))
    if (Test-Path $OutPutNameRMS){   Remove-Item $OutPutNameRMS   }

    $OutPutNameMLH = join-path $concatenatedFolder -ChildPath $(-join("MLH_Day_",$dayStart, "_", $dayEnd,"." , $mlhExt))
    if (Test-Path $OutPutNameMLH){   Remove-Item $OutPutNameMLH   }

    $OutPutNameCH2 = join-path $concatenatedFolder -ChildPath $(-join("CH2_Day_",$dayStart, "_", $dayEnd,"." , $ch2Ext))
    if (Test-Path $OutPutNameCH2){   Remove-Item $OutPutNameCH2   }
   
foreach($day in $($dayStart..$dayEnd)){

        ##
        $IonTEC = $IonTECTable[$day]
        $IonTECTable[$day].FullName
        
      
        if ($IonTECTable[$day].FullName){

            Get-Content $IonTECTable[$day].FullName | Add-Content $OutPutNameTEC 

        }

        ##
        $IonRMS = $IonRMSTable[$day]
        $IonRMSTable[$day].FullName
        
      
        if ($IonRMSTable[$day].FullName){

            Get-Content $IonRMSTable[$day].FullName | Add-Content $OutPutNameRMS 

        }

        ##
        $IonMLH = $IonMLHTable[$day]
        $IonMLHTable[$day].FullName
        
      
        if ($IonMLHTable[$day].FullName){

            Get-Content $IonMLHTable[$day].FullName | Add-Content $OutPutNameMLH 

        }

        
        ##
        $IonCH2 = $IonCH2Table[$day]
        $IonCH2Table[$day].FullName
        
      
        if ($IonCH2Table[$day].FullName){

            Get-Content $IonCH2Table[$day].FullName | Add-Content $OutPutNameCH2 

        }
        
        
}

# 4
#########################################

$dayStart = 91
$dayEnd = $dayStart + 29


    $OutPutNameTEC = join-path $concatenatedFolder -ChildPath $(-join("TEC_Day_",$dayStart, "_", $dayEnd,"." , $tecExt))
    if (Test-Path $OutPutNameTEC){   Remove-Item $OutPutNameTEC   }

    $OutPutNameRMS = join-path $concatenatedFolder -ChildPath $(-join("RMS_Day_",$dayStart, "_", $dayEnd,"." , $rmsExt))
    if (Test-Path $OutPutNameRMS){   Remove-Item $OutPutNameRMS   }

    $OutPutNameMLH = join-path $concatenatedFolder -ChildPath $(-join("MLH_Day_",$dayStart, "_", $dayEnd,"." , $mlhExt))
    if (Test-Path $OutPutNameMLH){   Remove-Item $OutPutNameMLH   }

    $OutPutNameCH2 = join-path $concatenatedFolder -ChildPath $(-join("CH2_Day_",$dayStart, "_", $dayEnd,"." , $ch2Ext))
    if (Test-Path $OutPutNameCH2){   Remove-Item $OutPutNameCH2   }
   
foreach($day in $($dayStart..$dayEnd)){

        ##
        $IonTEC = $IonTECTable[$day]
        $IonTECTable[$day].FullName
        
      
        if ($IonTECTable[$day].FullName){

            Get-Content $IonTECTable[$day].FullName | Add-Content $OutPutNameTEC 

        }

        ##
        $IonRMS = $IonRMSTable[$day]
        $IonRMSTable[$day].FullName
        
      
        if ($IonRMSTable[$day].FullName){

            Get-Content $IonRMSTable[$day].FullName | Add-Content $OutPutNameRMS 

        }

        ##
        $IonMLH = $IonMLHTable[$day]
        $IonMLHTable[$day].FullName
        
      
        if ($IonMLHTable[$day].FullName){

            Get-Content $IonMLHTable[$day].FullName | Add-Content $OutPutNameMLH 

        }

        
        ##
        $IonCH2 = $IonCH2Table[$day]
        $IonCH2Table[$day].FullName
        
      
        if ($IonCH2Table[$day].FullName){

            Get-Content $IonCH2Table[$day].FullName | Add-Content $OutPutNameCH2 

        }
        
        
}

# 5
#########################################

$dayStart = 121
$dayEnd = $dayStart + 29


    $OutPutNameTEC = join-path $concatenatedFolder -ChildPath $(-join("TEC_Day_",$dayStart, "_", $dayEnd,"." , $tecExt))
    if (Test-Path $OutPutNameTEC){   Remove-Item $OutPutNameTEC   }

    $OutPutNameRMS = join-path $concatenatedFolder -ChildPath $(-join("RMS_Day_",$dayStart, "_", $dayEnd,"." , $rmsExt))
    if (Test-Path $OutPutNameRMS){   Remove-Item $OutPutNameRMS   }

    $OutPutNameMLH = join-path $concatenatedFolder -ChildPath $(-join("MLH_Day_",$dayStart, "_", $dayEnd,"." , $mlhExt))
    if (Test-Path $OutPutNameMLH){   Remove-Item $OutPutNameMLH   }

    $OutPutNameCH2 = join-path $concatenatedFolder -ChildPath $(-join("CH2_Day_",$dayStart, "_", $dayEnd,"." , $ch2Ext))
    if (Test-Path $OutPutNameCH2){   Remove-Item $OutPutNameCH2   }
   
foreach($day in $($dayStart..$dayEnd)){

        ##
        $IonTEC = $IonTECTable[$day]
        $IonTECTable[$day].FullName
        
      
        if ($IonTECTable[$day].FullName){

            Get-Content $IonTECTable[$day].FullName | Add-Content $OutPutNameTEC 

        }

        ##
        $IonRMS = $IonRMSTable[$day]
        $IonRMSTable[$day].FullName
        
      
        if ($IonRMSTable[$day].FullName){

            Get-Content $IonRMSTable[$day].FullName | Add-Content $OutPutNameRMS 

        }

        ##
        $IonMLH = $IonMLHTable[$day]
        $IonMLHTable[$day].FullName
        
      
        if ($IonMLHTable[$day].FullName){

            Get-Content $IonMLHTable[$day].FullName | Add-Content $OutPutNameMLH 

        }

        
        ##
        $IonCH2 = $IonCH2Table[$day]
        $IonCH2Table[$day].FullName
        
      
        if ($IonCH2Table[$day].FullName){

            Get-Content $IonCH2Table[$day].FullName | Add-Content $OutPutNameCH2 

        }
        
        
}

# 6
#########################################

$dayStart = 151
$dayEnd = $dayStart + 29


    $OutPutNameTEC = join-path $concatenatedFolder -ChildPath $(-join("TEC_Day_",$dayStart, "_", $dayEnd,"." , $tecExt))
    if (Test-Path $OutPutNameTEC){   Remove-Item $OutPutNameTEC   }

    $OutPutNameRMS = join-path $concatenatedFolder -ChildPath $(-join("RMS_Day_",$dayStart, "_", $dayEnd,"." , $rmsExt))
    if (Test-Path $OutPutNameRMS){   Remove-Item $OutPutNameRMS   }

    $OutPutNameMLH = join-path $concatenatedFolder -ChildPath $(-join("MLH_Day_",$dayStart, "_", $dayEnd,"." , $mlhExt))
    if (Test-Path $OutPutNameMLH){   Remove-Item $OutPutNameMLH   }

    $OutPutNameCH2 = join-path $concatenatedFolder -ChildPath $(-join("CH2_Day_",$dayStart, "_", $dayEnd,"." , $ch2Ext))
    if (Test-Path $OutPutNameCH2){   Remove-Item $OutPutNameCH2   }
   
foreach($day in $($dayStart..$dayEnd)){

        ##
        $IonTEC = $IonTECTable[$day]
        $IonTECTable[$day].FullName
        
      
        if ($IonTECTable[$day].FullName){

            Get-Content $IonTECTable[$day].FullName | Add-Content $OutPutNameTEC 

        }

        ##
        $IonRMS = $IonRMSTable[$day]
        $IonRMSTable[$day].FullName
        
      
        if ($IonRMSTable[$day].FullName){

            Get-Content $IonRMSTable[$day].FullName | Add-Content $OutPutNameRMS 

        }

        ##
        $IonMLH = $IonMLHTable[$day]
        $IonMLHTable[$day].FullName
        
      
        if ($IonMLHTable[$day].FullName){

            Get-Content $IonMLHTable[$day].FullName | Add-Content $OutPutNameMLH 

        }

        
        ##
        $IonCH2 = $IonCH2Table[$day]
        $IonCH2Table[$day].FullName
        
      
        if ($IonCH2Table[$day].FullName){

            Get-Content $IonCH2Table[$day].FullName | Add-Content $OutPutNameCH2 

        }
        
        
}

# 7
#########################################

$dayStart = 181
$dayEnd = $dayStart + 29


    $OutPutNameTEC = join-path $concatenatedFolder -ChildPath $(-join("TEC_Day_",$dayStart, "_", $dayEnd,"." , $tecExt))
    if (Test-Path $OutPutNameTEC){   Remove-Item $OutPutNameTEC   }

    $OutPutNameRMS = join-path $concatenatedFolder -ChildPath $(-join("RMS_Day_",$dayStart, "_", $dayEnd,"." , $rmsExt))
    if (Test-Path $OutPutNameRMS){   Remove-Item $OutPutNameRMS   }

    $OutPutNameMLH = join-path $concatenatedFolder -ChildPath $(-join("MLH_Day_",$dayStart, "_", $dayEnd,"." , $mlhExt))
    if (Test-Path $OutPutNameMLH){   Remove-Item $OutPutNameMLH   }

    $OutPutNameCH2 = join-path $concatenatedFolder -ChildPath $(-join("CH2_Day_",$dayStart, "_", $dayEnd,"." , $ch2Ext))
    if (Test-Path $OutPutNameCH2){   Remove-Item $OutPutNameCH2   }
   
foreach($day in $($dayStart..$dayEnd)){

        ##
        $IonTEC = $IonTECTable[$day]
        $IonTECTable[$day].FullName
        
      
        if ($IonTECTable[$day].FullName){

            Get-Content $IonTECTable[$day].FullName | Add-Content $OutPutNameTEC 

        }

        ##
        $IonRMS = $IonRMSTable[$day]
        $IonRMSTable[$day].FullName
        
      
        if ($IonRMSTable[$day].FullName){

            Get-Content $IonRMSTable[$day].FullName | Add-Content $OutPutNameRMS 

        }

        ##
        $IonMLH = $IonMLHTable[$day]
        $IonMLHTable[$day].FullName
        
      
        if ($IonMLHTable[$day].FullName){

            Get-Content $IonMLHTable[$day].FullName | Add-Content $OutPutNameMLH 

        }

        
        ##
        $IonCH2 = $IonCH2Table[$day]
        $IonCH2Table[$day].FullName
        
      
        if ($IonCH2Table[$day].FullName){

            Get-Content $IonCH2Table[$day].FullName | Add-Content $OutPutNameCH2 

        }
        
        
}

# 8
#########################################

$dayStart = 211
$dayEnd = $dayStart + 29


    $OutPutNameTEC = join-path $concatenatedFolder -ChildPath $(-join("TEC_Day_",$dayStart, "_", $dayEnd,"." , $tecExt))
    if (Test-Path $OutPutNameTEC){   Remove-Item $OutPutNameTEC   }

    $OutPutNameRMS = join-path $concatenatedFolder -ChildPath $(-join("RMS_Day_",$dayStart, "_", $dayEnd,"." , $rmsExt))
    if (Test-Path $OutPutNameRMS){   Remove-Item $OutPutNameRMS   }

    $OutPutNameMLH = join-path $concatenatedFolder -ChildPath $(-join("MLH_Day_",$dayStart, "_", $dayEnd,"." , $mlhExt))
    if (Test-Path $OutPutNameMLH){   Remove-Item $OutPutNameMLH   }

    $OutPutNameCH2 = join-path $concatenatedFolder -ChildPath $(-join("CH2_Day_",$dayStart, "_", $dayEnd,"." , $ch2Ext))
    if (Test-Path $OutPutNameCH2){   Remove-Item $OutPutNameCH2   }
   
foreach($day in $($dayStart..$dayEnd)){

        ##
        $IonTEC = $IonTECTable[$day]
        $IonTECTable[$day].FullName
        
      
        if ($IonTECTable[$day].FullName){

            Get-Content $IonTECTable[$day].FullName | Add-Content $OutPutNameTEC 

        }

        ##
        $IonRMS = $IonRMSTable[$day]
        $IonRMSTable[$day].FullName
        
      
        if ($IonRMSTable[$day].FullName){

            Get-Content $IonRMSTable[$day].FullName | Add-Content $OutPutNameRMS 

        }

        ##
        $IonMLH = $IonMLHTable[$day]
        $IonMLHTable[$day].FullName
        
      
        if ($IonMLHTable[$day].FullName){

            Get-Content $IonMLHTable[$day].FullName | Add-Content $OutPutNameMLH 

        }

        
        ##
        $IonCH2 = $IonCH2Table[$day]
        $IonCH2Table[$day].FullName
        
      
        if ($IonCH2Table[$day].FullName){

            Get-Content $IonCH2Table[$day].FullName | Add-Content $OutPutNameCH2 

        }
        
        
}

# 9
#########################################

$dayStart = 241
$dayEnd = $dayStart + 29


    $OutPutNameTEC = join-path $concatenatedFolder -ChildPath $(-join("TEC_Day_",$dayStart, "_", $dayEnd,"." , $tecExt))
    if (Test-Path $OutPutNameTEC){   Remove-Item $OutPutNameTEC   }

    $OutPutNameRMS = join-path $concatenatedFolder -ChildPath $(-join("RMS_Day_",$dayStart, "_", $dayEnd,"." , $rmsExt))
    if (Test-Path $OutPutNameRMS){   Remove-Item $OutPutNameRMS   }

    $OutPutNameMLH = join-path $concatenatedFolder -ChildPath $(-join("MLH_Day_",$dayStart, "_", $dayEnd,"." , $mlhExt))
    if (Test-Path $OutPutNameMLH){   Remove-Item $OutPutNameMLH   }

    $OutPutNameCH2 = join-path $concatenatedFolder -ChildPath $(-join("CH2_Day_",$dayStart, "_", $dayEnd,"." , $ch2Ext))
    if (Test-Path $OutPutNameCH2){   Remove-Item $OutPutNameCH2   }
   
foreach($day in $($dayStart..$dayEnd)){

        ##
        $IonTEC = $IonTECTable[$day]
        $IonTECTable[$day].FullName
        
      
        if ($IonTECTable[$day].FullName){

            Get-Content $IonTECTable[$day].FullName | Add-Content $OutPutNameTEC 

        }

        ##
        $IonRMS = $IonRMSTable[$day]
        $IonRMSTable[$day].FullName
        
      
        if ($IonRMSTable[$day].FullName){

            Get-Content $IonRMSTable[$day].FullName | Add-Content $OutPutNameRMS 

        }

        ##
        $IonMLH = $IonMLHTable[$day]
        $IonMLHTable[$day].FullName
        
      
        if ($IonMLHTable[$day].FullName){

            Get-Content $IonMLHTable[$day].FullName | Add-Content $OutPutNameMLH 

        }

        
        ##
        $IonCH2 = $IonCH2Table[$day]
        $IonCH2Table[$day].FullName
        
      
        if ($IonCH2Table[$day].FullName){

            Get-Content $IonCH2Table[$day].FullName | Add-Content $OutPutNameCH2 

        }
        
        
}

# 10
#########################################

$dayStart = 271
$dayEnd = $dayStart + 29


    $OutPutNameTEC = join-path $concatenatedFolder -ChildPath $(-join("TEC_Day_",$dayStart, "_", $dayEnd,"." , $tecExt))
    if (Test-Path $OutPutNameTEC){   Remove-Item $OutPutNameTEC   }

    $OutPutNameRMS = join-path $concatenatedFolder -ChildPath $(-join("RMS_Day_",$dayStart, "_", $dayEnd,"." , $rmsExt))
    if (Test-Path $OutPutNameRMS){   Remove-Item $OutPutNameRMS   }

    $OutPutNameMLH = join-path $concatenatedFolder -ChildPath $(-join("MLH_Day_",$dayStart, "_", $dayEnd,"." , $mlhExt))
    if (Test-Path $OutPutNameMLH){   Remove-Item $OutPutNameMLH   }

    $OutPutNameCH2 = join-path $concatenatedFolder -ChildPath $(-join("CH2_Day_",$dayStart, "_", $dayEnd,"." , $ch2Ext))
    if (Test-Path $OutPutNameCH2){   Remove-Item $OutPutNameCH2   }
   
foreach($day in $($dayStart..$dayEnd)){

        ##
        $IonTEC = $IonTECTable[$day]
        $IonTECTable[$day].FullName
        
      
        if ($IonTECTable[$day].FullName){

            Get-Content $IonTECTable[$day].FullName | Add-Content $OutPutNameTEC 

        }

        ##
        $IonRMS = $IonRMSTable[$day]
        $IonRMSTable[$day].FullName
        
      
        if ($IonRMSTable[$day].FullName){

            Get-Content $IonRMSTable[$day].FullName | Add-Content $OutPutNameRMS 

        }

        ##
        $IonMLH = $IonMLHTable[$day]
        $IonMLHTable[$day].FullName
        
      
        if ($IonMLHTable[$day].FullName){

            Get-Content $IonMLHTable[$day].FullName | Add-Content $OutPutNameMLH 

        }

        
        ##
        $IonCH2 = $IonCH2Table[$day]
        $IonCH2Table[$day].FullName
        
      
        if ($IonCH2Table[$day].FullName){

            Get-Content $IonCH2Table[$day].FullName | Add-Content $OutPutNameCH2 

        }
        
        
}

# 11
#########################################

$dayStart = 301
$dayEnd = $dayStart + 29


    $OutPutNameTEC = join-path $concatenatedFolder -ChildPath $(-join("TEC_Day_",$dayStart, "_", $dayEnd,"." , $tecExt))
    if (Test-Path $OutPutNameTEC){   Remove-Item $OutPutNameTEC   }

    $OutPutNameRMS = join-path $concatenatedFolder -ChildPath $(-join("RMS_Day_",$dayStart, "_", $dayEnd,"." , $rmsExt))
    if (Test-Path $OutPutNameRMS){   Remove-Item $OutPutNameRMS   }

    $OutPutNameMLH = join-path $concatenatedFolder -ChildPath $(-join("MLH_Day_",$dayStart, "_", $dayEnd,"." , $mlhExt))
    if (Test-Path $OutPutNameMLH){   Remove-Item $OutPutNameMLH   }

    $OutPutNameCH2 = join-path $concatenatedFolder -ChildPath $(-join("CH2_Day_",$dayStart, "_", $dayEnd,"." , $ch2Ext))
    if (Test-Path $OutPutNameCH2){   Remove-Item $OutPutNameCH2   }
   
foreach($day in $($dayStart..$dayEnd)){

        ##
        $IonTEC = $IonTECTable[$day]
        $IonTECTable[$day].FullName
        
      
        if ($IonTECTable[$day].FullName){

            Get-Content $IonTECTable[$day].FullName | Add-Content $OutPutNameTEC 

        }

        ##
        $IonRMS = $IonRMSTable[$day]
        $IonRMSTable[$day].FullName
        
      
        if ($IonRMSTable[$day].FullName){

            Get-Content $IonRMSTable[$day].FullName | Add-Content $OutPutNameRMS 

        }

        ##
        $IonMLH = $IonMLHTable[$day]
        $IonMLHTable[$day].FullName
        
      
        if ($IonMLHTable[$day].FullName){

            Get-Content $IonMLHTable[$day].FullName | Add-Content $OutPutNameMLH 

        }

        
        ##
        $IonCH2 = $IonCH2Table[$day]
        $IonCH2Table[$day].FullName
        
      
        if ($IonCH2Table[$day].FullName){

            Get-Content $IonCH2Table[$day].FullName | Add-Content $OutPutNameCH2 

        }
        
        
}

# 12
#########################################

$dayStart = 331
$dayEnd = 366


    $OutPutNameTEC = join-path $concatenatedFolder -ChildPath $(-join("TEC_Day_",$dayStart, "_", $dayEnd,"." , $tecExt))
    if (Test-Path $OutPutNameTEC){   Remove-Item $OutPutNameTEC   }

    $OutPutNameRMS = join-path $concatenatedFolder -ChildPath $(-join("RMS_Day_",$dayStart, "_", $dayEnd,"." , $rmsExt))
    if (Test-Path $OutPutNameRMS){   Remove-Item $OutPutNameRMS   }

    $OutPutNameMLH = join-path $concatenatedFolder -ChildPath $(-join("MLH_Day_",$dayStart, "_", $dayEnd,"." , $mlhExt))
    if (Test-Path $OutPutNameMLH){   Remove-Item $OutPutNameMLH   }

    $OutPutNameCH2 = join-path $concatenatedFolder -ChildPath $(-join("CH2_Day_",$dayStart, "_", $dayEnd,"." , $ch2Ext))
    if (Test-Path $OutPutNameCH2){   Remove-Item $OutPutNameCH2   }
   
foreach($day in $($dayStart..$dayEnd)){

        ##
        $IonTEC = $IonTECTable[$day]
        $IonTECTable[$day].FullName
        
      
        if ($IonTECTable[$day].FullName){

            Get-Content $IonTECTable[$day].FullName | Add-Content $OutPutNameTEC 

        }

        ##
        $IonRMS = $IonRMSTable[$day]
        $IonRMSTable[$day].FullName
        
      
        if ($IonRMSTable[$day].FullName){

            Get-Content $IonRMSTable[$day].FullName | Add-Content $OutPutNameRMS 

        }

        ##
        $IonMLH = $IonMLHTable[$day]
        $IonMLHTable[$day].FullName
        
      
        if ($IonMLHTable[$day].FullName){

            Get-Content $IonMLHTable[$day].FullName | Add-Content $OutPutNameMLH 

        }

        
        ##
        $IonCH2 = $IonCH2Table[$day]
        $IonCH2Table[$day].FullName
        
      
        if ($IonCH2Table[$day].FullName){

            Get-Content $IonCH2Table[$day].FullName | Add-Content $OutPutNameCH2 

        }
        
        
}
