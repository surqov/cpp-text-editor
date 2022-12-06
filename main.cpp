#include <iostream>
#include <list>
#include <string>

using namespace std::literals;

class Editor {
public:
    Editor() = default;

    // сдвинуть курсор влево
    void Left() {
        if (cursor != text.begin()){
            std::list<char>::iterator curr_cursor = cursor--;
            std::swap(*cursor, *curr_cursor);
        }
    }

    // сдвинуть курсор вправо 
    void Right() {
        auto next_curr = cursor;
        ++next_curr;
        if (next_curr != text.end()) {
            std::list<char>::iterator curr_cursor = cursor++;
            std::swap(*cursor, *curr_cursor);
            ++next_curr;
        }
    }

    // вставить символ token
    void Insert(char token) {
        text.insert(cursor, token);
    }

    // вырезать не более tokens символов, начиная с текущей позиции курсора
    void Cut(size_t tokens = 1) {
        Copy(tokens);
        std::list<char>::iterator temp_cursor = cursor;
        std::list<char>::iterator curr_cursor = cursor;
        ++curr_cursor;
        while((tokens != 0) && (curr_cursor != text.end())) {
            --tokens;
            ++curr_cursor;
        }
        text.erase(++temp_cursor, curr_cursor);
    }

    // cкопировать не более tokens символов, начиная с текущей позиции курсора
    void Copy(size_t tokens = 1) {
        std::list<char>::iterator curr_cursor = cursor;
        ++curr_cursor;
        while ((tokens > 0) && (curr_cursor != text.end())) {
            --tokens;
            ++curr_cursor;
        }
        buff.assign(++cursor, curr_cursor);
        --cursor;
    }

    // вставить содержимое буфера в текущую позицию курсора
    void Paste() {
        text.insert(cursor, buff.begin(), buff.end());    
        buff = {};
    }

    // получить текущее содержимое текстового редактора
    std::string GetText() const {
        std::string out;
        for (auto it = text.begin(); it != text.end(); ++it) {
            if (it != cursor) {
                out += *it;
            }
        }
        return out;
    }
    
private:
    std::list<char> text = {'|'};
    std::list<char> buff = {};
    std::list<char>::iterator cursor = text.begin();
}; 

int main() {
    Editor editor;
    const std::string text = "hello, world"s;
    for (char c : text) {
        editor.Insert(c);
    }
    // Текущее состояние редактора: `hello, world|`
    for (size_t i = 0; i < text.size(); ++i) {
        editor.Left();
    }
    // Текущее состояние редактора: `|hello, world`
    editor.Cut(7);
    // Текущее состояние редактора: `|world`
    // в буфере обмена находится текст `hello, `
    for (size_t i = 0; i < 5; ++i) {
        editor.Right();
    }
    // Текущее состояние редактора: `world|`
    editor.Insert(',');
    editor.Insert(' ');
    // Текущее состояние редактора: `world, |`
    editor.Paste();
    // Текущее состояние редактора: `world, hello, |`
    editor.Left();
    editor.Left();
    //Текущее состояние редактора: `world, hello|, `
    editor.Cut(3);  // Будут вырезаны 2 символа
    // Текущее состояние редактора: `world, hello|`
    std::cout << editor.GetText();
    return 0;
} 