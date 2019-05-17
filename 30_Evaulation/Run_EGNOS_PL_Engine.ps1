
$PSCommandPath
$PS_Main_path = $PSCommandPath | Split-Path -Parent

$lat = 44.436874 
$lon = 26.103955 
$height = 10 
$dtMatch = 3600
$dt = 3600 
$IntProb = 0.9973 
$Elev = 10 

$KlobbucharBool = 0 

$EGNOS_PL_Engine = join-path $PS_Main_path -ChildPath "Runnables"| join-path -ChildPath "EGNOS_PL_Engine.exe"


<#
$NavFile = 'C:\Users\lus2bp\Source\Repos\GINA\30_Evaulation\2018\nav\brdc0040.18n'

$day = [int]$NavFile.SubString(4,3)


$targetType = '-i'
$TargetFile = 'C:\Users\lus2bp\Source\Repos\GINA\30_Evaulation\2018\ems\y2018_d004_136.18I'
$reftType = '-i' 
$RefFile = 'C:\Users\lus2bp\Source\Repos\GINA\30_Evaulation\2018\code\CODG0040.18I'




$EGNOS_PL_Engine = join-path $PS_Main_path -ChildPath "Runnables"| join-path -ChildPath "EGNOS_PL_Engine.exe"

$EGNOS_PL_Engine
$PL_Path_Base = join-path $PS_Main_path -ChildPath "2018" | join-path -ChildPath "pl" 

$EGNOS_PL_Engine
$PL_Path_Base

if(!$(Test-Path $PL_Path_Base)){
    New-Item -Path $PL_Path_Base -ItemType Directory
}


$subFolder = $(-join("IONEX-IONEX_lat",$lat, "_lon",$lon, "_height",$height, "_dt",$dt,"_intProb", $IntProb, "_elev", $Elev, "_KloboBool_",$KlobbucharBool))
$PL_Path = join-path $PL_Path_Base -ChildPath $subFolder


if(!$(Test-Path $PL_Path)){
    New-Item -Path $PL_Path -ItemType Directory
}


$OutputFileName = join-path $PL_Path -ChildPath $(-join("PL_", "day_", $day, "_lat",$lat, "_lon",$lon, "_height",$height, "_dt",$dt,"_intProb", $IntProb, "_elev", $Elev, "_KloboBool_",$KlobbucharBool, ".pl"))



$PL_Path_Base
$PL_Pat
$subFolder
$OutputFileName

#$lat, $lon, $height, $dt, $IntProb, $Elev, $KlobbucharBool, $OutputFileName, $NavFile, $targetType, $TargetFile, $reftType, $RefFile

#& "$EGNOS_PL_Engine" @($lat, $lon, $height, $dt, $IntProb, $Elev, $KlobbucharBool, $OutputFileName, $NavFile, $targetType, $TargetFile, $reftType, $RefFile)

#>

# Get Target file names and make hash table basod on day

$targetType = '-i'

$TargetFolder = join-path $PS_Main_path -ChildPath "2018" | join-path -ChildPath  "ems"
$TargetFiles = Get-ChildItem $TargetFolder  -Filter *.18I
$TargetTable =  @{}

 $TargetFiles | %{   

    $file = $_
    $day = [int]$file.Name.SubString(7,3)

    $TargetTable.add($day, $file)

}


# Get Reference file names and make hash table basod on day

$reftType = '-i' 

$ReferenceFolder = join-path $PS_Main_path -ChildPath "2018" | join-path -ChildPath  "code"
$ReferenceFiles = Get-ChildItem $ReferenceFolder  -Filter *.18I
$ReferenceTable =  @{}

 $ReferenceFiles | %{   

    $file = $_
    $day = [int]$file.Name.SubString(4,3)

    $ReferenceTable.add($day, $file)

}

# Get Navigation file names and make hash table basod on day
$NavFolder = join-path $PS_Main_path -ChildPath "2018" | join-path -ChildPath  "nav"
$NavFiles = Get-ChildItem $NavFolder  -Filter *.18n
$NavTable =  @{}

 $NavFiles | %{   

    $file = $_
    $day = [int]$file.Name.SubString(4,3)

    $NavTable.add($day, $file)

}

$PL_Path_Base = join-path $PS_Main_path -ChildPath "2018" | join-path -ChildPath "pl" 
$subFolder = $(-join("IONEX-IONEX_","_lat",$lat, "_lon",$lon, "_height",$height, "_dtMatch", $dtMatch, "_dt",$dt,"_intProb", $IntProb, "_elev", $Elev, "_KloboBool_",$KlobbucharBool))
$PL_Path = join-path $PL_Path_Base -ChildPath $subFolder


if(!$(Test-Path $PL_Path_Base)){
    New-Item -Path $PL_Path_Base -ItemType Directory
}

if(!$(Test-Path $PL_Path)){
    New-Item -Path $PL_Path -ItemType Directory
}

#44.436874 26.103955 10 3600 3600 0.9973 10 0 C:\Users\lus2bp\Source\Repos\GINA\30_Evaulation\2018\pl\PL_day_5_lat44436874_lon26103955_height10_dt3600_intProb09973_elev10_KloboBool_0.pl C:\Users\lus2bp\Source\Repos\GINA\30_Evaulation\2018\nav\brdc0050.18n -i C:\Users\lus2bp\Source\Repos\GINA\30_Evaulation\2018\ems\y2018_d005_136.18I -i C:\Users\lus2bp\Source\Repos\GINA\30_Evaulation\2018\code\CODG0050.18I

foreach($day in $(42..365)){

    if ($NavTable[$day] -and $ReferenceTable[$day] -and $TargetTable[$day]){
        
        $OutputFileName = join-path $PL_Path -ChildPath $(-join("PL_", "day_", $day, "_lat",$lat, "_lon",$lon, "_height",$height, "_dtMatch", $dtMatch, "_dt",$dt,"_intProb", $IntProb, "_elev", $Elev, "_KloboBool_",$KlobbucharBool, ".pl"))
        
        $NavFile = $NavTable[$day].FullName
        $TargetFile = $TargetTable[$day].FullName
        $RefFile = $ReferenceTable[$day].FullName

       
        #@($lat, $lon, $height, $dtMatch, $dt, $IntProb, $Elev, $KlobbucharBool, $OutputFileName, $NavFile, $targetType, $TargetFile, $reftType, $RefFile)

        #$OutputFileName
        & "$EGNOS_PL_Engine" @($lat, $lon, $height, $dtMatch, $dt, $IntProb, $Elev, $KlobbucharBool, $OutputFileName, $NavFile, $targetType, $TargetFile, $reftType, $RefFile)
    }
}