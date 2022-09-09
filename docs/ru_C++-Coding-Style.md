
- Language: Russian
- Charset:  UTF8


## 1. Отступы

В качестве отступа применять 4 пробела


## 2. Пробелы

### 2.1. Внутри скобок нет пробелов до и после скобок

```C++
  for ( int i=0; i<50; i++ )          // Bad
  writeHeader( &header, headerSize ); // Bad

  for (int i=0; i<50; i++)          // Good
  writeHeader(&header, headerSize); // Good
```

### 2.2. Пробел после ключевого слова for/while/if/catch


```C++
  if(!cin.eof())                   // Bad
  catch(const std::runtime_error&) // Bad

  if (!cin.eof())                   // Good
  catch (const std::runtime_error&) // Good
```

### 2.3. Пробел до и после операторов

```C++
  a=b+2;     // Bad
  a = b + 2; // Good
```

## 3. Расстановка фигурных скобок

### 3.1. Заголовок функции и открывающая фигурная на разных строках

```C++
Bad:
  void do_something() {
      // more lines
  }

Good
  void do_something() 
  {
      // more lines
  }
```

### 3.2. Имя класса или структуры и открывающая фигурная на разных строках

```C++
  class Image {  // Bad
      // more lines
  };

  class Image    // Good
  {
      // more lines
  };
```

### 3.3. В случае использования одного namespace открывающая фигурная ставится на новой строке

```C++
  namespace PE { // Bad
      // more lines
  };

  namespace PE   // Good
  {
      // more lines
  };
```

### 3.4. В случае использования нескольких namespace открывающая фигурная на той же строке

```C++
  namespace PE        // Bad
  {
  namespace Import
  {
      // more lines
  };
  };

  namespace PE {      // Good
  namespace Import {
       // more lines
  };
  };
```

### 3.5. Для нескольких namespace закрывающие фигурные скобки но одной строке

```C++
  namespace PE {      // Bad
  namespace Import {
       // more lines
  }; // Import
  }; // PE

  namespace PE {      // Good
  namespace Import {
       // more lines
  }} // PE::Import
```

### 3.6. Тело управляющей структуры и открывающая фигурная на одной строке

Ключевые слова : `try`, `catch`, `if`, `while`, `for`, `do`, `else`, `switch`, `case`

```C++
  if (!file.end())                // Bad
  {
       // more lines
  }

  catch (const std::exception&)   // Bad
  {
       // more lines
  }

  if (!file.end()) {              // Good
       // more lines
  }

  catch (const std::exception&) { // Good
       // more lines
  }
```

### 3.7. Не ставить скобок если тело управляющей конструкции из одной строки

Ключевые слова : `if`, `while`, `for`

```C++
  if (!file.end()) {              // Bad
      superHeader->write();
  }

  for (int i=0; i<10; ++i) {      // Bad
      print(superHeader[i].magic);
  }

  if (!file.end())                // Good
      superHeader->write();

  for (int i=0; i<10; ++i)        // Good
      print(superHeader[i].magic);
```

### 3.8. Закрывающая скобка и следующее ключевое слово на разных строках

Ключевое слово : `else`, `catch`

```C++
  if (header.isEmpty()) {
      // do something
  } else {                // Bad
      // do something
  }

  if (header.isEmpty()) {
      // do something
  }
  else {                  // Good
      // do something
  }
```

## 4. Названия

### 4.1. Названия переменных поясняется существительным

### 4.2. Названия типов начинаются с прописной буквы

```C++
  class header; // Bad
  class Header; // Good
```

### 4.3. В названиях методов применяется глагол

```C++
   header();  // Bad
   entropy(); // Bad
   empty();   // Bad

   getHeader();      // Good
   computeEntropy(); // Good
   isEmpty();        // Good
```

### 4.4. Названия методов поясняются глаголом и возможным существительным

```C++
  write();       // Good
  replaceChar(); // Good
  getPointer();  // Good
```

### 4.5. Методы вопросы должны иметь в имени is/has/have

```C++
  empty();   // Bad
  isEmpty(); // Good
```

### 4.6. Несколько слов пишутся слитно и в верблюжьем стиле

```C++
  write_header(); // Bad
  writeHeader();  // Good
```

### 4.7. Название метода начинается со строчной буквы

```C++
  WriteHeader(); // Bad
  writeHeader(); // Good
```

### 4.8. typedef-типы должны завершаться постфиксом '_t'

```C++
  typedef uint32_t Rva;   // Bad
  typedef uint32_t Rva_t; // Good
```

### 4.9. Названия namespace-ов пишутся в нижнем регистре

```C++
namespace PE { // Bad
}

namespace pe { // Good
}
```

### 4.10. Два слова в названии namespace пишутся раздельно через '_'

```C++
namespace HeadersUtils { // Bad
}

namespace headers_utils { // Good
}
```

## 5. Макросы

### 5.1. Наименования макросов пишутся в верхнем регистре

```C++
# if !defined(super_header_hpp) // Bad
# if !defined(SUPER_HEADER_HPP) // Good
```

### 5.2. Название Include guards на основании имен библиотек и файла

Предположим название библиотеки PE, а имя файла `image.hpp`, тогда:

```C++
#if !defined(PE_IMAGE_HPP)
#define      PE_IMAGE_HPP
// more lines
#endif // PE_IMAGE_HPP
```

## 6. GOTO

### 6.1. Стремиться избегать

По возможности вместо этой конструкции использовать другие : `break`, `continue` и др.

### 6.2. В случае необходимости GOTO

Метка должна быть в верхнем регистре и начинаться с новой строки в самом начале

Bad:
```C++
  if (header.magic == SuperType::HeaderOne)
      goto exit;
  // more lines
  exit:
```

Good:
```C++
  if (header.magic == SuperType::HeaderOne)
      goto EXIT;
  // more lines
  EXIT:
```

### 6.3. Вместо макро-констант использовать C++ константы

```C++
  #define MAGIC 0x200      // Bad
  const int Magic = 0x200; // Good

  #define HEADER_TYPE1 = 0x1000;  // Bad
  #define HEADER_TYPE2 = 0x2000; 

  enum HeaderType {               // Good
     Type1 = 0x1000,
     Type2 = 0x2000,
  }
```

## 7. Пустые строки

### 7.1. Между функциями одна пустая строка

### 7.2. Между началом\завершением namespace и кодом две пустые строки

### 7.3. Между началом файла и первой строкой кода одна пустая строка

## 8. Комментарии

### 8.1. Ставится комментарий после закрывающей скобки namespace 

```C++
  namespace PE {      // Bad
  namespace Import {
       // more lines
  }}

  namespace PE {      // Good
  namespace Import {
       // more lines
  }} PE::Import
```
