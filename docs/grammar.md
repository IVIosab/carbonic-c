# Carbonic-C Grammar
 A brief overview of the language specifications



## Formal grammar
**Program :**  {SimpleDeclaration | RoutineDecleration}
**SimpleDecleration :** {VariableDecleration| TypeDecleration}
**VariableDeclaration :**
```
var Identifier : Type [ is Expression ]
var Identifier is Expression
```

**TypeDeclaration:**
```
type Identifier is Type
```
**RoutineDeclaration:**
```
routine Identifier ( Parameters ) [ : Type ] is
 Body
 end
```
**Parameters :** ParameterDeclaration { , ParameterDeclaration }
**ParameterDeclaration:**
```
Identifier : Type
```
**Type :**
> PrimitiveType
 ArrayType
 RecordType
 Identifier

**PrimitiveType:**
>integer
real
boolean
char

**RecordType :**
```
record { VariableDeclaration } end
```
**ArrayType :**
```
array [ Expression ] Type
```
**Body : ** { SimpleDeclaration | Statement }
**Statement :**
> Assignment
RoutineCall
WhileLoop
ForLoop
/* ForeachLoop*/
IfStatement

**Assignment :**
```
ModifiablePrimary := Expression
```
**RoutineCall : **
```
Identifier [ ( Expression { , Expression } ) ]
```
**WhileLoop :**
```
while Expression
loop
Body
end
```
**ForLoop : **
```
for Identifier Range
loop
Body
end
```
**Range :**
```
in [ reverse ] Expression .. Expression
```
/*
ForeachLoop : foreach Identifier from ModifiablePrimary loop
 Body
 end
*/
**IfStatement :**
```
if Expression
then
Body
[ else Body ]
end
```
**Expression :** Relation { ( and | or | xor ) Relation }
**Relation :** Simple [ ( < | <= | > | >= | = | /= ) Simple ]
**Simple :** Factor { ( * | / | % | ^ ) Factor }
__Factor :__ Summand { ( + | - ) Summand }
__Summand :__ Primary | ( Expression )

**Primary :**
> IntegralLiteral
 | RealLiteral
 | true | false
 | ModifiablePrimary

**ModifiablePrimary** : Identifier { . Identifier | [ Expression ] }

## Collaboration
You can share this doc using https://writemd.xyz/d/63eb8c0ac23439156.
- Asem Abdelhady
- Jaafar Totanji
- Menna Awadallah
- Mosab Mohamed
## Code example:

```
type int is integer;
routine main () : int is
 var x : int is (7 + 9);
 if (x >= 9) then
  print("x is bigger than 9\n");
 else
  print("x is not bigger than 9\n");
 end
end
```
## Variables (new)

--var family_name: Dursley
When Mr and Mrs {{family_name}} woke up
