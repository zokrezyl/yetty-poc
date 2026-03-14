#!/usr/bin/env python3
"""
Demo 09: Todo List

Dynamic widget creation - adding/removing todo items.
"""

import sys
sys.path.insert(0, '../../../src/ygui-bindings/python')

from ygui import Engine, EventType

class TodoApp:
    def __init__(self):
        self.engine = Engine(width=500, height=600)
        self.todos = []
        self.next_id = 0
        self.start_y = 100

        # Title
        self.engine.label("title", x=30, y=20, text="Todo List")

        # Add button
        self.engine.button("add_btn", x=30, y=50, w=100, h=32, label="+ Add Task")

        # Clear completed button
        self.engine.button("clear_btn", x=150, y=50, w=140, h=32, label="Clear Completed")

        # Stats label
        self.stats_label = self.engine.label("stats", x=310, y=58, text="0 items")

        self.engine.on_event(self.on_event)

    def add_todo(self, text):
        todo_id = self.next_id
        self.next_id += 1
        y = self.start_y + len(self.todos) * 40

        # Checkbox for completion
        cb = self.engine.checkbox(
            f"todo_cb_{todo_id}",
            x=30, y=y, w=24, h=24,
            label="", checked=False
        )

        # Task label
        label = self.engine.label(
            f"todo_text_{todo_id}",
            x=60, y=y + 4,
            text=text
        )

        # Delete button
        del_btn = self.engine.button(
            f"todo_del_{todo_id}",
            x=400, y=y, w=60, h=28,
            label="Delete"
        )

        self.todos.append({
            'id': todo_id,
            'text': text,
            'completed': False,
            'checkbox': cb,
            'label': label,
            'delete_btn': del_btn
        })

        self.update_stats()
        self.engine.rebuild()
        print(f"Added todo: {text}")

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
        self.engine.rebuild()

    def reposition_todos(self):
        for i, todo in enumerate(self.todos):
            y = self.start_y + i * 40
            todo['checkbox'].set_position(30, y)
            todo['label'].set_position(60, y + 4)
            todo['delete_btn'].set_position(400, y)

    def clear_completed(self):
        completed_ids = [t['id'] for t in self.todos if t['completed']]
        for todo_id in completed_ids:
            self.remove_todo(todo_id)
        print(f"Cleared {len(completed_ids)} completed items")

    def update_stats(self):
        total = len(self.todos)
        completed = sum(1 for t in self.todos if t['completed'])
        self.stats_label.set_text(f"{completed}/{total} done".encode('utf-8'))

    def on_event(self, event):
        if event.type == EventType.CLICK:
            if event.widget_id == "add_btn":
                self.add_todo(f"Task {self.next_id + 1}")
            elif event.widget_id == "clear_btn":
                self.clear_completed()
            elif event.widget_id.startswith("todo_del_"):
                todo_id = int(event.widget_id.split("_")[-1])
                self.remove_todo(todo_id)

        elif event.type == EventType.CHANGE:
            if event.widget_id.startswith("todo_cb_"):
                todo_id = int(event.widget_id.split("_")[-1])
                for todo in self.todos:
                    if todo['id'] == todo_id:
                        todo['completed'] = todo['checkbox'].checked
                        status = "completed" if todo['completed'] else "pending"
                        print(f"Todo '{todo['text']}' marked as {status}")
                        break
                self.update_stats()

    def run(self):
        self.engine.rebuild()

        # Demo: add some todos
        self.add_todo("Buy groceries")
        self.add_todo("Write documentation")
        self.add_todo("Review pull request")
        self.add_todo("Fix bug #42")

        # Complete some tasks
        for todo in self.todos[:2]:
            todo['checkbox'].set_checked(True)
            todo['completed'] = True
        self.update_stats()

        print(f"\nTodos: {len(self.todos)}")
        for todo in self.todos:
            status = "✓" if todo['completed'] else " "
            print(f"  [{status}] {todo['text']}")

def main():
    app = TodoApp()
    app.run()

if __name__ == "__main__":
    main()
