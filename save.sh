# echo "enter your commit text > "
# read text
echo $1
git add .
git commit -m '$1'
git push origin
