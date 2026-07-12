/* CHAOS relay — displays the latest status line jessy writes to
 * /ext/apps_data/chaos_relay/status.txt over the existing USB-serial link
 * (flipper-tool.py push). First Track A app: proves the ufbt build/flash
 * pipeline and the jessy -> Flipper bridge before any bigger firmware work.
 */
#include <furi.h>
#include <gui/gui.h>
#include <input/input.h>
#include <storage/storage.h>

#include <chaos_relay_icons.h>

#define STATUS_PATH      EXT_PATH("apps_data/chaos_relay/status.txt")
#define STATUS_MAX_LEN   128
#define POLL_INTERVAL_MS 1000

typedef struct {
    FuriMutex* mutex;
    char status[STATUS_MAX_LEN];
    bool status_loaded;
} ChaosRelayModel;

static void chaos_relay_read_status(ChaosRelayModel* model, Storage* storage) {
    File* file = storage_file_alloc(storage);
    char buf[STATUS_MAX_LEN];
    size_t read = 0;
    bool opened = storage_file_open(file, STATUS_PATH, FSAM_READ, FSOM_OPEN_EXISTING);
    if(opened) {
        read = storage_file_read(file, buf, STATUS_MAX_LEN - 1);
    }
    storage_file_close(file);
    storage_file_free(file);

    furi_mutex_acquire(model->mutex, FuriWaitForever);
    if(opened && read > 0) {
        while(read > 0 && (buf[read - 1] == '\n' || buf[read - 1] == '\r')) {
            read--;
        }
        memcpy(model->status, buf, read);
        model->status[read] = '\0';
        model->status_loaded = true;
    } else {
        model->status_loaded = false;
    }
    furi_mutex_release(model->mutex);
}

static void chaos_relay_draw_callback(Canvas* canvas, void* context) {
    ChaosRelayModel* model = context;
    canvas_clear(canvas);

    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str(canvas, 2, 10, "CHAOS relay");

    canvas_set_font(canvas, FontSecondary);
    furi_mutex_acquire(model->mutex, FuriWaitForever);
    canvas_draw_str(canvas, 2, 24, model->status_loaded ? model->status : "Waiting for jessy...");
    furi_mutex_release(model->mutex);

    canvas_draw_str(canvas, 2, 60, "Back: exit");
}

static void chaos_relay_input_callback(InputEvent* event, void* context) {
    FuriMessageQueue* queue = context;
    furi_message_queue_put(queue, event, FuriWaitForever);
}

int32_t chaos_relay_app(void* p) {
    UNUSED(p);

    ChaosRelayModel model = {0};
    model.mutex = furi_mutex_alloc(FuriMutexTypeNormal);

    FuriMessageQueue* event_queue = furi_message_queue_alloc(8, sizeof(InputEvent));

    ViewPort* view_port = view_port_alloc();
    view_port_draw_callback_set(view_port, chaos_relay_draw_callback, &model);
    view_port_input_callback_set(view_port, chaos_relay_input_callback, event_queue);

    Gui* gui = furi_record_open(RECORD_GUI);
    gui_add_view_port(gui, view_port, GuiLayerFullscreen);

    Storage* storage = furi_record_open(RECORD_STORAGE);

    bool running = true;
    while(running) {
        chaos_relay_read_status(&model, storage);
        view_port_update(view_port);

        InputEvent event;
        FuriStatus status = furi_message_queue_get(event_queue, &event, POLL_INTERVAL_MS);
        if(status == FuriStatusOk && event.type == InputTypeShort && event.key == InputKeyBack) {
            running = false;
        }
    }

    furi_record_close(RECORD_STORAGE);
    gui_remove_view_port(gui, view_port);
    furi_record_close(RECORD_GUI);
    view_port_free(view_port);
    furi_message_queue_free(event_queue);
    furi_mutex_free(model.mutex);

    return 0;
}
