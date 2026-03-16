#!/usr/bin/env python3
"""
Demo 09: Todo List

Dynamic widget creation - adding/removing todo items.
"""

import ygui

class TodoApp:
    def __init__(self):
        ygui.init()
        self.engine = ygui.Engine("todo-app", x=2, y=2, width=500, height=500)
        self.todos = []
        self.next_id = 0
        self.start_y = 100

        # Title
        self.engine.label("title", 30, 20, "Todo List")

        # Stats label
        self.stats_label = self.engine.label("stats", 320, 58, "0 items")

        # Add button
        add_btn = self.engine.button("add_btn", 30, 50, 100, 35, "+ Add Task")
        add_btn.on_click(self.on_add)

        # Clear completed button
        clear_btn = self.engine.button("clear_btn", 150, 50, 140, 35, "Clear Done")
        clear_btn.on_click(self.on_clear_completed)

    def on_add(self):
        self.add_todo(f"Task {self.next_id + 1}")

    def on_clear_completed(self):
        completed_ids = [t['id'] for t in self.todos if t['completed']]
        for todo_id in completed_ids:
            self.remove_todo(todo_id)

    def add_todo(self, text):
        todo_id = self.next_id
        self.next_id += 1
        y = self.start_y + len(self.todos) * 45

        # Checkbox for completion
        cb = self.engine.checkbox(f"todo_cb_{todo_id}", 30, y, 30, 30, "", False)

        # Task label
        label = self.engine.label(f"todo_text_{todo_id}", 70, y + 6, text)

        # Delete button
        del_btn = self.engine.button(f"todo_del_{todo_id}", 400, y, 65, 30, "Delete")

        todo = {
            'id': todo_id,
            'text': text,
            'completed': False,
            'checkbox': cb,
            'label': label,
            'delete_btn': del_btn
        }

        # Set callbacks
        def make_toggle(t):
            def toggle(checked):
                t['completed'] = checked
                self.update_stats()
            return toggle

        def make_delete(tid):
            def delete():
                self.remove_todo(tid)
            return delete

        cb.on_change(make_toggle(todo))
        del_btn.on_click(make_delete(todo_id))

        self.todos.append(todo)
        self.update_stats()

    def remove_todo(self, todo_id):
        for i, todo in enumerate(self.todos):
            if todo['id'] == todo_id:
                todo['checkbox'].remove()
                todo['label'].remove()
                todo['delete_btn'].remove()
                self.todos.pop(i)
                break

        self.reposition_todos()
        self.update_stats()

    def reposition_todos(self):
        for i, todo in enumerate(self.todos):
            y = self.start_y + i * 45
            todo['checkbox'].set_position(30, y)
            todo['label'].set_position(70, y + 6)
            todo['delete_btn'].set_position(400, y)

    def update_stats(self):
        total = len(self.todos)
        completed = sum(1 for t in self.todos if t['completed'])
        self.stats_label.set_text(f"{completed}/{total} done")

    def run(self):
        def on_key(key, mods):
            if key == ord('q'):
                self.engine.stop()

        self.engine.on_key(on_key)

        # Add some initial todos
        self.add_todo("Buy groceries")
        self.add_todo("Write documentation")
        self.add_todo("Review pull request")

        self.engine.show()
        self.engine.run()

        ygui.shutdown()


def main():
    app = TodoApp()
    app.run()

if __name__ == "__main__":
    main()
