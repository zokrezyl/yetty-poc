#!/usr/bin/env python3
"""
Demo 10: Dashboard Application

A complete dashboard with multiple panels, real-time updates,
and various widget types working together.
"""

import sys
import random
import time
sys.path.insert(0, '../../../src/ygui-bindings/python')

from ygui import Engine, EventType

def rgb_to_abgr(r, g, b, a=255):
    return (a << 24) | (b << 16) | (g << 8) | r

class Dashboard:
    def __init__(self):
        self.engine = Engine(width=900, height=700)

        # Colors
        self.bg_dark = rgb_to_abgr(30, 30, 35)
        self.bg_panel = rgb_to_abgr(45, 45, 50)
        self.accent = rgb_to_abgr(0, 150, 255)
        self.success = rgb_to_abgr(50, 200, 100)
        self.warning = rgb_to_abgr(255, 180, 50)
        self.danger = rgb_to_abgr(255, 80, 80)

        # State
        self.cpu_usage = 45.0
        self.memory_usage = 62.0
        self.disk_usage = 78.0
        self.network_in = 0.0
        self.network_out = 0.0
        self.notifications_enabled = True
        self.dark_mode = True
        self.refresh_rate = 1.0

        self.setup_ui()
        self.engine.on_event(self.on_event)

    def setup_ui(self):
        # Header
        self.engine.label("header", x=30, y=20, text="System Dashboard")

        # Status indicator
        self.status_label = self.engine.label("status", x=750, y=20, text="● Online")

        # ===== System Stats Panel =====
        stats_panel = self.engine.panel("stats_panel", x=20, y=60, w=420, h=200)
        stats_panel.set_bg_color(self.bg_panel)

        self.engine.label("stats_title", x=35, y=75, text="System Resources")

        # CPU
        self.engine.label("cpu_label", x=35, y=110, text="CPU")
        self.cpu_bar = self.engine.progress("cpu_bar", x=100, y=108, w=280, h=18, value=0.45)
        self.cpu_value = self.engine.label("cpu_value", x=390, y=110, text="45%")

        # Memory
        self.engine.label("mem_label", x=35, y=145, text="Memory")
        self.mem_bar = self.engine.progress("mem_bar", x=100, y=143, w=280, h=18, value=0.62)
        self.mem_value = self.engine.label("mem_value", x=390, y=145, text="62%")

        # Disk
        self.engine.label("disk_label", x=35, y=180, text="Disk")
        self.disk_bar = self.engine.progress("disk_bar", x=100, y=178, w=280, h=18, value=0.78)
        self.disk_value = self.engine.label("disk_value", x=390, y=180, text="78%")

        # Network
        self.engine.label("net_label", x=35, y=215, text="Network")
        self.net_in = self.engine.label("net_in", x=100, y=215, text="↓ 0 KB/s")
        self.net_out = self.engine.label("net_out", x=220, y=215, text="↑ 0 KB/s")

        # ===== Quick Actions Panel =====
        actions_panel = self.engine.panel("actions_panel", x=460, y=60, w=420, h=200)
        actions_panel.set_bg_color(self.bg_panel)

        self.engine.label("actions_title", x=475, y=75, text="Quick Actions")

        self.engine.button("refresh", x=475, y=110, w=120, h=36, label="Refresh")
        self.engine.button("clear_cache", x=610, y=110, w=120, h=36, label="Clear Cache")
        self.engine.button("restart", x=745, y=110, w=120, h=36, label="Restart")

        self.engine.button("backup", x=475, y=160, w=120, h=36, label="Backup")
        self.engine.button("logs", x=610, y=160, w=120, h=36, label="View Logs")
        self.engine.button("settings", x=745, y=160, w=120, h=36, label="Settings")

        # Alert checkbox
        self.alert_cb = self.engine.checkbox(
            "alerts", x=475, y=210, w=200, h=24,
            label="Enable Alerts", checked=True
        )

        # ===== Settings Panel =====
        settings_panel = self.engine.panel("settings_panel", x=20, y=280, w=420, h=200)
        settings_panel.set_bg_color(self.bg_panel)

        self.engine.label("settings_title", x=35, y=295, text="Settings")

        # Theme toggle
        self.dark_mode_cb = self.engine.checkbox(
            "dark_mode", x=35, y=330, w=180, h=24,
            label="Dark Mode", checked=True
        )

        # Notifications toggle
        self.notif_cb = self.engine.checkbox(
            "notifications", x=35, y=365, w=180, h=24,
            label="Notifications", checked=True
        )

        # Refresh rate slider
        self.engine.label("refresh_label", x=35, y=410, text="Refresh Rate")
        self.refresh_slider = self.engine.slider(
            "refresh_rate", x=150, y=408, w=200, h=20,
            min_val=0.5, max_val=5.0, value=1.0
        )
        self.refresh_value = self.engine.label("refresh_val", x=360, y=410, text="1.0s")

        # ===== Processes Panel =====
        proc_panel = self.engine.panel("proc_panel", x=460, y=280, w=420, h=200)
        proc_panel.set_bg_color(self.bg_panel)

        self.engine.label("proc_title", x=475, y=295, text="Top Processes")

        # Process list (simplified)
        processes = [
            ("chrome", "12.5%", "1.2 GB"),
            ("code", "8.3%", "890 MB"),
            ("python", "5.1%", "256 MB"),
            ("docker", "3.2%", "512 MB"),
        ]

        for i, (name, cpu, mem) in enumerate(processes):
            y = 330 + i * 30
            self.engine.label(f"proc_{i}_name", x=475, y=y, text=name)
            self.engine.label(f"proc_{i}_cpu", x=600, y=y, text=cpu)
            self.engine.label(f"proc_{i}_mem", x=700, y=y, text=mem)

        # ===== Footer =====
        self.engine.label("footer", x=30, y=660, text="Dashboard v1.0 | Last update: just now")

        # Apply/Cancel buttons
        self.engine.button("apply", x=650, y=500, w=100, h=36, label="Apply")
        self.engine.button("cancel", x=770, y=500, w=100, h=36, label="Cancel")

    def update_stats(self):
        """Simulate real-time updates."""
        # Random fluctuations
        self.cpu_usage = max(5, min(95, self.cpu_usage + random.uniform(-5, 5)))
        self.memory_usage = max(20, min(90, self.memory_usage + random.uniform(-2, 2)))
        self.network_in = random.uniform(0, 1000)
        self.network_out = random.uniform(0, 500)

        # Update UI
        self.cpu_bar.set_value(self.cpu_usage / 100)
        self.cpu_value.set_text(f"{self.cpu_usage:.0f}%".encode('utf-8'))

        self.mem_bar.set_value(self.memory_usage / 100)
        self.mem_value.set_text(f"{self.memory_usage:.0f}%".encode('utf-8'))

        self.net_in.set_text(f"↓ {self.network_in:.0f} KB/s".encode('utf-8'))
        self.net_out.set_text(f"↑ {self.network_out:.0f} KB/s".encode('utf-8'))

    def on_event(self, event):
        if event.type == EventType.CLICK:
            action = event.widget_id
            if action == "refresh":
                print("Refreshing data...")
                self.update_stats()
            elif action == "clear_cache":
                print("Cache cleared!")
            elif action == "restart":
                print("Restarting services...")
            elif action == "backup":
                print("Starting backup...")
            elif action == "logs":
                print("Opening logs...")
            elif action == "settings":
                print("Opening settings...")
            elif action == "apply":
                print("Settings applied!")
                print(f"  Dark mode: {self.dark_mode_cb.checked}")
                print(f"  Notifications: {self.notif_cb.checked}")
                print(f"  Refresh rate: {self.refresh_slider.value:.1f}s")
            elif action == "cancel":
                print("Changes cancelled")

        elif event.type == EventType.CHANGE:
            if event.widget_id == "refresh_rate":
                val = self.refresh_slider.value
                self.refresh_value.set_text(f"{val:.1f}s".encode('utf-8'))
            elif event.widget_id == "dark_mode":
                mode = "dark" if self.dark_mode_cb.checked else "light"
                print(f"Theme: {mode} mode")
            elif event.widget_id == "notifications":
                status = "enabled" if self.notif_cb.checked else "disabled"
                print(f"Notifications {status}")
            elif event.widget_id == "alerts":
                status = "enabled" if self.alert_cb.checked else "disabled"
                print(f"Alerts {status}")

    def run(self):
        self.engine.rebuild()

        print("Dashboard running...")
        print("=" * 50)

        # Simulate a few update cycles
        for i in range(3):
            self.update_stats()
            print(f"Update {i+1}: CPU={self.cpu_usage:.1f}%, Mem={self.memory_usage:.1f}%")
            self.engine.rebuild()

        print("=" * 50)
        print("Dashboard demo complete!")


def main():
    dashboard = Dashboard()
    dashboard.run()


if __name__ == "__main__":
    main()
