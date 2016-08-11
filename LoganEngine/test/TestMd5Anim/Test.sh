ModelName="bob_lamp_update_export";
InputFiles=$(ls "./Files/${ModelName}")
ModelFile="./Models/${ModelName}.md5mesh"

for InputFile in $InputFiles;
do
  FilePath="./Files/${ModelName}/${InputFile}"
  #FileContent=$(cat "${FilePath}")
  echo "Testing file ${FilePath}"
  #cat $FilePath
  cat $FilePath | ./Test $ModelFile #> out.txt
  echo "Finished testing ${FilePath}
  "
done