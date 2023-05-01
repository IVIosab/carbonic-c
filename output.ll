; ModuleID = 'Program'
source_filename = "Program"

define double @main() {
entry:
  %x = alloca double, align 8
  store double 5.500000e+00, double* %x, align 8
  %x1 = load double, double* %x, align 8
  %result = fadd double 5.700000e+00, %x1
  ret double %result
}