SELECT * FROM alapanyag;
SELECT * FROM menu;
SELECT * FROM recept;

--1
(SELECT etterem_nev FROM menu WHERE etel_nev='zöldséges ragu') 
MINUS
(SELECT etterem_nev FROM menu WHERE etel_nev='palacsinta');

--2
SELECT menu.etterem_nev, COUNT(*) db FROM menu CROSS JOIN (SELECT DISTINCT recept.etel_nev FROM recept WHERE recept.alapanyag_nev='tojás') r2
WHERE menu.etel_nev = r2.etel_nev GROUP BY menu.etterem_nev;

--3
(SELECT DISTINCT recept.etel_nev FROM recept) MINUS
(SELECT DISTINCT recept.etel_nev FROM alapanyag CROSS JOIN recept WHERE alapanyag.kategoria='hús' AND alapanyag.alapanyag_nev=recept.alapanyag_nev);

--4
(SELECT etterem_nev FROM menu)
MINUS
(SELECT menu.etterem_nev FROM menu CROSS JOIN 
((SELECT DISTINCT recept.etel_nev FROM recept) MINUS (SELECT DISTINCT recept.etel_nev FROM alapanyag CROSS JOIN recept WHERE alapanyag.kategoria='gabona' AND alapanyag.alapanyag_nev=recept.alapanyag_nev)) t
WHERE t.etel_nev=menu.etel_nev
GROUP BY menu.etterem_nev
HAVING COUNT(*)>=2);

--5
SELECT etel_nev FROM recept
GROUP BY etel_nev
HAVING COUNT(*)=(SELECT MIN(db) FROM (SELECT etel_nev, COUNT(*) db FROM recept GROUP BY etel_nev));



