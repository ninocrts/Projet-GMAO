Set objExcel = CreateObject("Excel.Application")
objExcel.Visible = False  ' Empêche l'ouverture de la fenêtre Excel
Set objWorkbook = objExcel.Workbooks.Open("C:\Users\utilisateur\Desktop\EPF\PAML\Projet GMAO\Projet-GMAO\Macro.xlsm") ' Remplacez par le chemin de votre fichier Excel
objExcel.Run "maMacro" ' Remplacez par le nom de votre macro
objExcel.Quit
Set objWorkbook = Nothing
Set objExcel = Nothing