# Carbonic-C Grammar
 A brief overview of the language specifications

# Program start
Program automatically starts from main, unless another routine was specified to start with. Carbonic-C supports starting from any routine as well as some global statements (not specified in routines). Global statments are limited to declerations and would not support loops, if conditions, or print statments.

## Formal grammar
**Program :**  SimpleDeclaration | RoutineDecleration
**SimpleDecleration :** VariableDecleration | TypeDecleration
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
**Body : **  SimpleDeclaration | Statement
**Statement :**
> Assignment
RoutineCall
WhileLoop
ForLoop
ForeachLoop
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

**ForeachLoop :** 
```
foreach Identifier from ModifiablePrimary loop
 Body
 end
```

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
- Asem Abdelhady
- Jaffar Totanji
- Menna Awadallah
- Mosab Mohamed
## Code example:

```
type int is integer;
routine main () : int is
 var x : int is (7 + 9);
 if (x >= 9) then
  print('x is bigger than 9\n');
 else
  print('x is not bigger than 9\n');
 end
end
```