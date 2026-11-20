#!/usr/bin/env python3
# fit_remap.py — сверка meta_remap.h с реальными заголовками (cam_types/cam_intf old & new)
import re, sys

def load(f):
    return open(f, encoding="utf8", errors="replace").read()

def strip_comments(t):
    t = re.sub(r"/\*[\s\S]*?\*/", " ", t)
    return re.sub(r"//[^\n]*", " ", t)

def enums_with(t):
    out = []
    for m in re.finditer(r"typedef\s+enum\s*\{([\s\S]*?)\}\s*(\w+)\s*;", strip_comments(t)):
        d, val = {}, 0
        for line in m.group(1).split("\n"):
            line = line.strip().rstrip(",").strip()
            if not line:
                continue
            mm = re.match(r"^([A-Za-z_]\w*)(?:\s*=\s*(.+))?$", line)
            if not mm:
                continue
            nm, expr = mm.groups()
            if expr:
                try:
                    val = int(expr.strip().rstrip("UuLl"), 0)
                except ValueError:
                    pass
            if nm.startswith("CAM_INTF_"):
                d[nm] = val
            val += 1
        out.append(d)
    return out

def collect(files):
    d = {}
    for f in files:
        for blk in enums_with(load(f)):
            for k, v in blk.items():
                d.setdefault(k, v)
    return d

new_ids = collect(["cam_types_new.h", "cam_intf_new.h"])
old_ids = collect(["cam_types_old.h", "cam_intf_old.h"])
src = load("meta_remap.h")

# прямая таблица: [NEW_NAME] = { old_idx u, old_size u, CONV_* }
E_FWD = (r"\[\s*(CAM_INTF_\w+)\s*\]\s*=\s*\{"
         r"\s*(\d+)u\s*,\s*(\d+)u\s*,\s*(CONV_\w+)\s*\}")
fwd = re.findall(E_FWD, src)

# обратная таблица: [old_idx u] = { NEW_NAME, CONV_* }  (инвертированный формат)
E_REV = (r"\[\s*(\d+)u\s*\]\s*=\s*\{"
         r"\s*(CAM_INTF_\w+)\s*,\s*(CONV_\w+)\s*\}")
rev = re.findall(E_REV, src)

helpers = set(re.findall(r"member_variable_(CAM_INTF_\w+)", src))

problems = []

# 1) прямая таблица: имя должно существовать в новом заголовке,
#    а old_idx — совпадать со значением в старом
fwd_names = set()
for name, oid, osz, conv in fwd:
    fwd_names.add(name)
    if name not in new_ids:
        problems.append("УДАЛИТЬ %s: имени нет в новом заголовке (реверт убрал параметр)" % name)
    elif name in old_ids and old_ids[name] != int(oid):
        problems.append("ИСПРАВИТЬ %s: old_idx %s -> %d" % (name, oid, old_ids[name]))

# 2) обратная таблица: индекс должен совпадать со старым значением имени,
#    а имя — существовать в новом заголовке
for oid, name, conv in rev:
    if name not in new_ids:
        problems.append("УДАЛИТЬ rev %s: имени нет в новом заголовке" % name)
    elif name in old_ids and old_ids[name] != int(oid):
        problems.append("ИСПРАВИТЬ rev %s: индекс %s -> %d" % (name, oid, old_ids[name]))

# 3) switch-функции без записи в таблице
for n in sorted(helpers - fwd_names):
    problems.append("%s: есть в switch, нет в таблице" % n)

# 4) записи без switch-функции
for name, _, _, _ in fwd:
    if name not in helpers:
        problems.append("%s: есть в таблице, нет в switch" % name)

print("Прямая таблица g_remap: %d записей" % len(fwd))
print("Обратная таблица g_remap_rev: %d записей" % len(rev))
print("Helper-функций member_variable_*: %d" % len(helpers))
print()
if problems:
    print("\n".join(problems))
else:
    print("Таблицы консистентны с заголовками — правки не требуются")
