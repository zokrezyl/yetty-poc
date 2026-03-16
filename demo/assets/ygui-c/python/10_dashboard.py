#!/usr/bin/env python3
"""
Demo 10: Dashboard Application

A complete dashboard with multiple panels and various widget types.
"""

import ygui

def rgb_to_abgr(r, g, b, a=255):
    return (a << 24) | (b << 16) | (g << 8) | r

class Dashboard:
    def __init__(self):
        ygui.init()
        self.engine = ygui.Engine("dashboard", x=1, y=1, width=800, height=550)

        self.bg_panel = rgb_to_abgr(45, 45, 50)

        # State
        self.cpu_usage = 45.0
        self.memory_usage = 62.0
        self.disk_usage = 78.0

        self.setup_ui()

    def setup_ui(self):
        # Header
        self.engine.label("header", 30, 15, "System Dashboard")
        self.status_label = self.engine.label("status", 650, 15, "Online")

        # ===== System Stats Panel =====
        stats_panel = self.engine.panel("stats_panel", 20, 50, 370, 180)
        stats_panel.set_bg_color(self.bg_panel)

        self.engine.label("stats_title", 35, 65, "System Resources")

        # CPU
        self.engine.label("cpu_label", 35, 100, "CPU")
        self.cpu_bar = self.engine.progress("cpu_bar", 90, 98, 220, 18, 0.45)
        self.cpu_value = self.engine.label("cpu_value", 320, 100, "45%")

        # Memory
        self.engine.label("mem_label", 35, 130, "Memory")
        self.mem_bar = self.engine.progress("mem_bar", 90, 128, 220, 18, 0.62)
        self.mem_value = self.engine.label("mem_value", 320, 130, "62%")

        # Disk
        self.engine.label("disk_label", 35, 160, "Disk")
        self.disk_bar = self.engine.progress("disk_bar", 90, 158, 220, 18, 0.78)
        self.disk_value = self.engine.label("disk_value", 320, 160, "78%")

        # Network
        self.engine.label("net_label", 35, 195, "Network")
        self.net_in = self.engine.label("net_in", 90, 195, "In: 0 KB/s")
        self.net_out = self.engine.label("net_out", 200, 195, "Out: 0 KB/s")

        # ===== Quick Actions Panel =====
        actions_panel = self.engine.panel("actions_panel", 410, 50, 370, 180)
        actions_panel.set_bg_color(self.bg_panel)

        self.engine.label("actions_title", 425, 65, "Quick Actions")

        refresh_btn = self.engine.button("refresh", 425, 95, 100, 35, "Refresh")
        refresh_btn.on_click(self.on_refresh)

        cache_btn = self.engine.button("clear_cache", 540, 95, 100, 35, "Clear Cache")
        cache_btn.on_click(lambda: self.set_status("Cache cleared"))

        restart_btn = self.engine.button("restart", 655, 95, 100, 35, "Restart")
        restart_btn.on_click(lambda: self.set_status("Restarting..."))

        backup_btn = self.engine.button("backup", 425, 145, 100, 35, "Backup")
        backup_btn.on_click(lambda: self.set_status("Backup started"))

        logs_btn = self.engine.button("logs", 540, 145, 100, 35, "View Logs")
        logs_btn.on_click(lambda: self.set_status("Opening logs"))

        settings_btn = self.engine.button("settings", 655, 145, 100, 35, "Settings")
        settings_btn.on_click(lambda: self.set_status("Settings open"))

        # Alert checkbox
        self.alert_cb = self.engine.checkbox("alerts", 425, 195, 180, 25, "Enable Alerts", True)

        # ===== Settings Panel =====
        settings_panel = self.engine.panel("settings_panel", 20, 250, 370, 180)
        settings_panel.set_bg_color(self.bg_panel)

        self.engine.label("settings_title", 35, 265, "Settings")

        # Theme toggle
        self.dark_mode_cb = self.engine.checkbox("dark_mode", 35, 300, 160, 25, "Dark Mode", True)

        # Notifications toggle
        self.notif_cb = self.engine.checkbox("notifications", 35, 335, 160, 25, "Notifications", True)

        # Refresh rate slider
        self.engine.label("refresh_label", 35, 380, "Refresh:")
        self.refresh_slider = self.engine.slider("refresh_rate", 110, 378, 180, 22, 0.5, 5.0, 1.0)
        self.refresh_value = self.engine.label("refresh_val", 300, 380, "1.0s")

        self.refresh_slider.on_change(self.on_refresh_rate)

        # ===== Info Panel =====
        info_panel = self.engine.panel("info_panel", 410, 250, 370, 180)
        info_panel.set_bg_color(self.bg_panel)

        self.engine.label("info_title", 425, 265, "System Info")

        self.engine.label("host_label", 425, 300, "Host: workstation")
        self.engine.label("os_label", 425, 330, "OS: Linux 6.8")
        self.engine.label("uptime_label", 425, 360, "Uptime: 3d 12h 45m")

        # Apply/Quit buttons
        apply_btn = self.engine.button("apply", 550, 395, 100, 35, "Apply")
        apply_btn.on_click(self.on_apply)

        quit_btn = self.engine.button("quit", 665, 395, 100, 35, "Quit")
        quit_btn.on_click(lambda: self.engine.stop())

        # Footer
        self.engine.label("footer", 30, 510, "Dashboard v1.0")

    def set_status(self, text):
        self.status_label.set_text(text)

    def on_refresh(self):
        import random
        self.cpu_usage = random.uniform(20, 80)
        self.memory_usage = random.uniform(40, 85)

        self.cpu_bar.set_value(self.cpu_usage / 100)
        self.cpu_value.set_text(f"{int(self.cpu_usage)}%")

        self.mem_bar.set_value(self.memory_usage / 100)
        self.mem_value.set_text(f"{int(self.memory_usage)}%")

        net_in = random.uniform(0, 500)
        net_out = random.uniform(0, 200)
        self.net_in.set_text(f"In: {int(net_in)} KB/s")
        self.net_out.set_text(f"Out: {int(net_out)} KB/s")

        self.set_status("Refreshed")

    def on_refresh_rate(self, value):
        self.refresh_value.set_text(f"{value:.1f}s")

    def on_apply(self):
        dark = self.dark_mode_cb.checked
        notif = self.notif_cb.checked
        rate = self.refresh_slider.value
        self.set_status(f"Applied: dark={dark}, notif={notif}, rate={rate:.1f}s")

    def run(self):
        def on_key(key, mods):
            if key == ord('q'):
                self.engine.stop()

        self.engine.on_key(on_key)

        self.engine.show()
        self.engine.run()

        ygui.shutdown()


def main():
    dashboard = Dashboard()
    dashboard.run()


if __name__ == "__main__":
    main()
