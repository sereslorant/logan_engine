InputFiles=$(ls ./Files)

for InputFile in $InputFiles;
do
  FilePath="./Files/${InputFile}"
  #FileContent=$(cat "${FilePath}")
  echo "Testing file ${FilePath}"
  #cat $FilePath
  cat $FilePath | ./Test >> out.txt
  #cat $FilePath | ./Test
  echo "Finished testing ${FilePath}
  "
done
