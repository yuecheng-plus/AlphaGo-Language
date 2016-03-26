Var
  i,j,k,p,tot,n,t,m:longint;
  l,w:array[1..5006]of longint;
  f:array[0..5006]of longint;
Procedure qsort(ll,rr:longint);
  var
    mid,t,i,j,mid2:longint;
  begin
    i:=ll;j:=rr;
    mid:=l[(i+j)shr 1];
    mid2:=w[(i+j)shr 1];
    repeat
      while (l[i]<mid)or(l[i]=mid)and(w[i]<mid2) do inc(i);
      while (l[j]>mid)or(l[j]=mid)and(w[j]>mid2) do dec(j);
      if i<=j then
        begin
          t:=l[i];l[i]:=l[j];l[j]:=t;
          t:=w[i];w[i]:=w[j];w[j]:=t;
          inc(i);dec(j);
        end;
    until i>j;
    if i<rr then qsort(i,rr);
    if ll<j then qsort(ll,j);
  end;
Begin
  readln(t);
  for j:=1 to t do
    begin
      readln(n);
      for i:=1 to n do read(l[i],w[i]);
      qsort(1,n);
      tot:=1;
      f[1]:=w[1];
      for i:=2 to n do
        begin
          p:=0;
          for k:=1 to tot do
            if (f[k]<=w[i]) then
               begin
                 if p=0 then p:=k
                    else if f[p]<f[k]
                            then p:=k;
               end;
          if p=0 then
            begin
              inc(tot);
              f[tot]:=w[i];
            end
            else f[p]:=w[i];
        end;
      writeln(tot);
    end;
End.