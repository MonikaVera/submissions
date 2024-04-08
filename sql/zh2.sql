SELECT * FROM dolgozo;
/
--1
CREATE OR REPLACE FUNCTION nth(n INT) RETURN INT IS
    a1 INT := 1;
    a2 INT := 2;
BEGIN 
    FOR i IN 3..n LOOP
        IF MOD(i,2)=1 THEN
            a1 := (a1 + a2)*a1;
        ELSE
            a2 := (a1 + a2)*a2;
        END IF;
    END LOOP;
    IF MOD(n,2)=1 THEN
        RETURN a1;
    ELSE
        RETURN a2;
    END IF;
END nth;
/
SELECT nth(4) FROM dual;
SELECT nth(5) FROM dual;


--2
SELECT * FROM vpetya.magassag ORDER BY x;
/
CREATE OR REPLACE FUNCTION numofhills RETURN INT IS
    CURSOR curs1 IS SELECT * FROM vpetya.magassag ORDER BY x;
    prev1 INT := -1;
    prev2 INT := -1;
    numhills INT:=0;
BEGIN
    FOR rec1 IN curs1 LOOP
        IF prev1>0 AND prev2>0 AND prev1<prev2 AND prev2>rec1.y THEN
            numhills:=numhills+1;
        END IF;
        prev1:=prev2;
        prev2:=rec1.y;
    END LOOP;
    RETURN numhills;
END numofhills;
/
SELECT numofhills() FROM dual;

--3
CREATE TABLE Autok (
    a_id INT PRIMARY KEY,
    rendszam VARCHAR2(20),
    tulajdonos VARCHAR2(20),
    birsag INT
);

CREATE TABLE Traffipax (
    t_id INT,
    rendszam VARCHAR2(20),
    x INT,
    ido INT,
    CONSTRAINT tr FOREIGN KEY (t_id) REFERENCES Autok(a_id)
);

INSERT INTO Autok VALUES (1,'AAA-111', 'Kati', 0);
INSERT INTO Autok VALUES ((SELECT max(a_id)+1 FROM Autok),'AAA-112', 'Béla', 0);
INSERT INTO Autok VALUES ((SELECT max(a_id)+1 FROM Autok),'AAA-131', 'Csaba', 0);

SELECT * FROM autok;
SELECT * FROM traffipax;
SELECT * FROM autok CROSS JOIN traffipax WHERE autok.a_id=traffipax.t_id AND autok.birsag=0;

INSERT INTO Traffipax (t_id,x,ido) VALUES (1,0,0);
INSERT INTO Traffipax (t_id,x,ido) VALUES (2,3,3);
/
DECLARE 
    CURSOR curs1 IS SELECT * FROM autok CROSS JOIN traffipax WHERE autok.a_id=traffipax.t_id AND autok.birsag=0;
BEGIN
    FOR rec1 IN curs1 LOOP
        DELETE FROM autok WHERE rec1.a_id=autok.a_id;
    END LOOP;
END;
/

DROP TABLE autos;
CREATE TABLE AUTOS AS SELECT * FROM vpetya.autok;
CREATE TABLE tr AS SELECT * FROM vpetya.traffipax;
SELECT * FROM autos NATURAL jOIN tr;
--4
CREATE OR REPLACE FUNCTION func1 RETURN INT IS
     CURSOR curs1 IS SELECT * FROM traffipax FOR UPDATE;
     v INT:= 0;
BEGIN 
    FOR rec1 IN curs1 LOOP
        v:=rec1.x/rec1.ido*3600;
        IF v>130 THEN 
         UPDATE autok SET birsag=(v-130)*1500 WHERE autok;
        END IF;
    END LOOP;
END func1;