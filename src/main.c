/* #include "compiler/ast/ast.h" */
#include "compiler/lexer.h"
#include "compiler/parser.h"
/* #include <stdio.h> */

/* int partition(int numbers[], size_t len, size_t left, size_t right) { */
/*   int pivot = numbers[(left + right) / 2]; */
/*   int low = left; */
/*   int high = right; */

/*   while (low <= high) { */
/*     while (numbers[low] < pivot) { */
/*       low++; */
/*     } */

/*     while (numbers[high] > pivot) { */
/*       high--; */
/*     } */

/*     if (low <= high) { */
/*       int temp = numbers[low]; */
/*       numbers[low] = numbers[high]; */
/*       numbers[high] = temp; */
/*       low++; */
/*       high--; */
/*     } */
/*   } */

/*   return low; */
/* } */

/* void quick_sort(int numbers[], size_t len, size_t left, size_t right) { */
/*   int pivot_idx; */

/*   if (len > 1) { */
/*     pivot_idx = partition(numbers, len, left, right); */

/*     if (left < pivot_idx - 1) { */
/*       quick_sort(numbers, len, left, pivot_idx - 1); */
/*     } */

/*     if (pivot_idx < right) { */
/*       quick_sort(numbers, len, pivot_idx, right); */
/*     } */
/*   } */
/* } */

/* #define ARRAY_LEN(arr) sizeof(arr) / sizeof(arr[0]) */

int main() {
  /* int nums[] = {5, 1, 4, 3, 6, 2}; */
  /* size_t len = ARRAY_LEN(nums); */
  /* quick_sort(nums, len, 0, len - 1); */

  /* printf("["); */
  /* for (size_t i = 0; i < len; i++) { */
  /*   printf("%d,", nums[i]); */
  /* } */
  /* printf("]"); */

  char *input =
      "let some_number: int = 2; let w: int = 3; let name: string = "
      "\"jakis string\"; let is_ok: bool = true; let is_ok: bool = false;";
  Lexer *lexer = lexer_new(input);

  Parser *parser = parser_new(lexer);
  parser_parse_program(parser);

  return 0;
}
