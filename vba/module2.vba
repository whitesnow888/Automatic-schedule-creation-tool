Function CCount(Rng As Range, idx)
Dim R As Range
Dim Cnt As Long
Application.Volatile
For Each R In Rng
    If R.Interior.ColorIndex = idx Then Cnt = Cnt + 1
Next R
CCount = Cnt
End Function

Function GetIndx(Rng As Range)
If Rng.Count > 1 Then
    GetIndx = vbNullString
    Exit Function
End If
GetIndx = Rng.Interior.ColorIndex

End Function
