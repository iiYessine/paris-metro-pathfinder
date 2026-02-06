#include <gtk/gtk.h>
#include <string.h>
#include <math.h>
#include "frontend.h"
#include "backend.h"

/* ===== VARIABLES LOCALES ===== */

static GtkWidget *tree_depart;
static GtkWidget *tree_arrivee;
static GtkWidget *label_resultat;
static GtkWidget *search_depart;
static GtkWidget *search_arrivee;
static GtkWidget *selection_window = NULL;
static GtkWidget *result_window = NULL;

static GtkListStore *store_stations;
static GtkTreeModelFilter *filter_depart;
static GtkTreeModelFilter *filter_arrivee;

static int depart_id = -1;
static int arrivee_id = -1;
static int restart_search = 0;

enum {
    COL_ID,
    COL_NOM,
    N_COLS
};

/* ===== STUNNING CSS STYLING ===== */

static void apply_css(void) {
    GtkCssProvider *provider = gtk_css_provider_new();
    
    const char *css_data = 
        "@import url('https://fonts.googleapis.com/css2?family=Orbitron:wght@400;700;900&family=Inter:wght@300;400;600&display=swap');"
        ""
        "* {"
        "   font-family: 'Inter', -apple-system, sans-serif;"
        "}"
        ""
        "window {"
        "   background: linear-gradient(135deg, #0f0c29 0%, #302b63 50%, #24243e 100%);"
        "}"
        ""
        "#main-container {"
        "   background: rgba(255, 255, 255, 0.03);"
        "   border-radius: 30px;"
        "   border: 1px solid rgba(255, 255, 255, 0.1);"
        "   box-shadow: 0 30px 90px rgba(0, 0, 0, 0.5),"
        "               inset 0 1px 1px rgba(255, 255, 255, 0.1);"
        "}"
        ""
        "#cosmic-header {"
        "   background: linear-gradient(135deg, #667eea 0%, #764ba2 50%, #f093fb 100%);"
        "   border-radius: 30px 30px 0 0;"
        "   padding: 35px;"
        "   box-shadow: 0 10px 40px rgba(102, 126, 234, 0.3);"
        "}"
        ""
        "#titulo {"
        "   font-family: 'Orbitron', monospace;"
        "   font-size: 42px;"
        "   font-weight: 900;"
        "   color: white;"
        "   text-shadow: 0 0 20px rgba(255, 255, 255, 0.5),"
        "                0 0 40px rgba(102, 126, 234, 0.5),"
        "                2px 2px 4px rgba(0, 0, 0, 0.3);"
        "   letter-spacing: 2px;"
        "}"
        ""
        "#subtitle {"
        "   font-size: 16px;"
        "   font-weight: 300;"
        "   color: rgba(255, 255, 255, 0.9);"
        "   letter-spacing: 3px;"
        "}"
        ""
        "#metro-icon {"
        "   font-size: 72px;"
        "   color: white;"
        "   text-shadow: 0 0 30px rgba(255, 255, 255, 0.8),"
        "                0 0 60px rgba(240, 147, 251, 0.6);"
        "}"
        ""
        "#section-card {"
        "   background: rgba(255, 255, 255, 0.05);"
        "   border-radius: 20px;"
        "   padding: 25px;"
        "   border: 1px solid rgba(255, 255, 255, 0.1);"
        "   box-shadow: 0 8px 32px rgba(0, 0, 0, 0.3);"
        "}"
        ""
        "#section-label {"
        "   font-size: 18px;"
        "   font-weight: 600;"
        "   color: #f093fb;"
        "   letter-spacing: 2px;"
        "   text-shadow: 0 0 10px rgba(240, 147, 251, 0.5);"
        "}"
        ""
        "treeview {"
        "   border-radius: 15px;"
        "   border: 2px solid rgba(102, 126, 234, 0.3);"
        "   background: rgba(15, 12, 41, 0.4);"
        "   color: rgba(255, 255, 255, 0.9);"
        "}"
        ""
        "treeview:selected {"
        "   background: linear-gradient(90deg, #667eea 0%, #764ba2 100%);"
        "   color: white;"
        "}"
        ""
        "treeview header button {"
        "   background: linear-gradient(135deg, rgba(102, 126, 234, 0.4), rgba(118, 75, 162, 0.4));"
        "   font-weight: 700;"
        "   color: white;"
        "   border: none;"
        "   padding: 15px;"
        "   text-transform: uppercase;"
        "   letter-spacing: 1px;"
        "}"
        ""
        "entry {"
        "   border-radius: 15px;"
        "   border: 2px solid rgba(102, 126, 234, 0.4);"
        "   padding: 15px 20px;"
        "   font-size: 15px;"
        "   min-height: 50px;"
        "   background: rgba(15, 12, 41, 0.6);"
        "   color: white;"
        "   caret-color: #f093fb;"
        "}"
        ""
        "entry::placeholder {"
        "   color: rgba(255, 255, 255, 0.5);"
        "}"
        ""
        "entry:focus {"
        "   border-color: #f093fb;"
        "   background: rgba(15, 12, 41, 0.8);"
        "   box-shadow: 0 0 0 4px rgba(240, 147, 251, 0.2),"
        "               0 0 30px rgba(240, 147, 251, 0.3);"
        "}"
        ""
        "#btn-validate {"
        "   background: linear-gradient(135deg, #667eea 0%, #764ba2 50%, #f093fb 100%);"
        "   color: white;"
        "   font-family: 'Orbitron', monospace;"
        "   font-size: 18px;"
        "   font-weight: 700;"
        "   border-radius: 15px;"
        "   padding: 20px 60px;"
        "   border: 2px solid rgba(255, 255, 255, 0.3);"
        "   min-height: 65px;"
        "   letter-spacing: 2px;"
        "   box-shadow: 0 10px 40px rgba(102, 126, 234, 0.4),"
        "               inset 0 1px 1px rgba(255, 255, 255, 0.3);"
        "}"
        ""
        "#btn-validate:hover {"
        "   background: linear-gradient(135deg, #f093fb 0%, #764ba2 50%, #667eea 100%);"
        "   box-shadow: 0 15px 60px rgba(240, 147, 251, 0.6),"
        "               0 0 30px rgba(255, 255, 255, 0.4),"
        "               inset 0 1px 1px rgba(255, 255, 255, 0.4);"
        "}"
        ""
        "#btn-restart {"
        "   background: linear-gradient(135deg, #667eea 0%, #764ba2 50%, #f093fb 100%);"
        "   color: white;"
        "   font-family: 'Orbitron', monospace;"
        "   font-size: 14px;"
        "   font-weight: 700;"
        "   border-radius: 12px;"
        "   padding: 12px 30px;"
        "   border: 2px solid rgba(255, 255, 255, 0.3);"
        "   min-height: 45px;"
        "   letter-spacing: 1px;"
        "   box-shadow: 0 10px 40px rgba(102, 126, 234, 0.4),"
        "               inset 0 1px 1px rgba(255, 255, 255, 0.3);"
        "}"
        ""
        "#btn-restart:hover {"
        "   background: linear-gradient(135deg, #f093fb 0%, #764ba2 50%, #667eea 100%);"
        "   box-shadow: 0 15px 60px rgba(240, 147, 251, 0.6),"
        "               0 0 30px rgba(255, 255, 255, 0.4),"
        "               inset 0 1px 1px rgba(255, 255, 255, 0.4);"
        "}"
        ""
        "#btn-close {"
        "   background: linear-gradient(135deg, #eb3349 0%, #f45c43 100%);"
        "   color: white;"
        "   font-family: 'Orbitron', monospace;"
        "   font-size: 16px;"
        "   font-weight: 700;"
        "   border-radius: 12px;"
        "   padding: 16px 40px;"
        "   border: 2px solid rgba(255, 255, 255, 0.2);"
        "   min-height: 55px;"
        "   letter-spacing: 1px;"
        "}"
        ""
        "#btn-close:hover {"
        "   background: linear-gradient(135deg, #f45c43 0%, #eb3349 100%);"
        "   box-shadow: 0 10px 40px rgba(235, 51, 73, 0.5);"
        "}"
        ""
        "#error-label {"
        "   color: #ff6b9d;"
        "   font-size: 15px;"
        "   font-weight: 600;"
        "   text-shadow: 0 0 10px rgba(255, 107, 157, 0.5);"
        "}"
        ""
        "#result-container {"
        "   background: rgba(255, 255, 255, 0.03);"
        "   border-radius: 30px;"
        "   border: 1px solid rgba(255, 255, 255, 0.1);"
        "}"
        ""
        "#journey-header {"
        "   background: linear-gradient(135deg, #667eea 0%, #764ba2 50%, #f093fb 100%);"
        "   border-radius: 25px;"
        "   padding: 40px;"
        "   box-shadow: 0 15px 60px rgba(102, 126, 234, 0.4),"
        "               inset 0 1px 1px rgba(255, 255, 255, 0.2);"
        "}"
        ""
        "#station-badge {"
        "   background: rgba(255, 255, 255, 0.15);"
        "   backdrop-filter: blur(10px);"
        "   border-radius: 20px;"
        "   padding: 25px 35px;"
        "   border: 2px solid rgba(255, 255, 255, 0.3);"
        "   box-shadow: 0 8px 32px rgba(0, 0, 0, 0.3);"
        "}"
        ""
        "#station-label {"
        "   color: white;"
        "   font-size: 22px;"
        "   font-weight: 600;"
        "   text-shadow: 0 2px 4px rgba(0, 0, 0, 0.3);"
        "}"
        ""
        "#station-name {"
        "   color: white;"
        "   font-family: 'Orbitron', monospace;"
        "   font-size: 32px;"
        "   font-weight: 900;"
        "   text-shadow: 0 0 20px rgba(255, 255, 255, 0.4),"
        "                0 2px 4px rgba(0, 0, 0, 0.4);"
        "   letter-spacing: 1px;"
        "}"
        ""
        "#glow-arrow {"
        "   font-size: 64px;"
        "   color: white;"
        "   text-shadow: 0 0 40px rgba(255, 255, 255, 0.8),"
        "                0 0 80px rgba(240, 147, 251, 0.6);"
        "}"
        ""
        "#time-card {"
        "   font-size: 26px;"
        "   font-weight: 700;"
        "   color: white;"
        "   background: linear-gradient(135deg, #11998e 0%, #38ef7d 100%);"
        "   border: 2px solid rgba(255, 255, 255, 0.4);"
        "   border-radius: 20px;"
        "   padding: 25px 45px;"
        "   box-shadow: 0 10px 40px rgba(17, 153, 142, 0.4),"
        "               inset 0 1px 1px rgba(255, 255, 255, 0.3);"
        "   text-shadow: 0 2px 4px rgba(0, 0, 0, 0.2);"
        "}"
        ""
        "#details-title {"
        "   font-family: 'Orbitron', monospace;"
        "   font-size: 28px;"
        "   font-weight: 700;"
        "   color: #f093fb;"
        "   text-transform: uppercase;"
        "   letter-spacing: 2px;"
        "   text-shadow: 0 0 20px rgba(240, 147, 251, 0.5);"
        "}"
        ""
        "separator {"
        "   background: linear-gradient(90deg, "
        "       transparent 0%, "
        "       rgba(102, 126, 234, 0.5) 50%, "
        "       transparent 100%);"
        "   min-height: 2px;"
        "}";
    
    gtk_css_provider_load_from_data(provider, css_data, -1, NULL);
    
    gtk_style_context_add_provider_for_screen(
        gdk_screen_get_default(),
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
    );
    
    g_object_unref(provider);
}

/* ===== FILTRAGE DES STATIONS ===== */

static gboolean filter_func_depart(GtkTreeModel *model, GtkTreeIter *iter, gpointer data) {
    const char *search_text = gtk_entry_get_text(GTK_ENTRY(search_depart));
    
    if (!search_text || strlen(search_text) == 0)
        return TRUE;
    
    char *nom;
    gtk_tree_model_get(model, iter, COL_NOM, &nom, -1);
    
    gboolean visible = (nom && strcasestr(nom, search_text) != NULL);
    g_free(nom);
    
    return visible;
}

static gboolean filter_func_arrivee(GtkTreeModel *model, GtkTreeIter *iter, gpointer data) {
    const char *search_text = gtk_entry_get_text(GTK_ENTRY(search_arrivee));
    
    if (!search_text || strlen(search_text) == 0)
        return TRUE;
    
    char *nom;
    gtk_tree_model_get(model, iter, COL_NOM, &nom, -1);
    
    gboolean visible = (nom && strcasestr(nom, search_text) != NULL);
    g_free(nom);
    
    return visible;
}

static void on_search_changed_depart(GtkEntry *entry, gpointer data) {
    gtk_tree_model_filter_refilter(filter_depart);
}

static void on_search_changed_arrivee(GtkEntry *entry, gpointer data) {
    gtk_tree_model_filter_refilter(filter_arrivee);
}

/* ===== OUTILS ===== */

static int get_selected_id(GtkTreeView *tree) {
    GtkTreeSelection *selection = gtk_tree_view_get_selection(tree);
    GtkTreeModel *model;
    GtkTreeIter iter;

    if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
        int id;
        gtk_tree_model_get(model, &iter, COL_ID, &id, -1);
        return id;
    }
    return -1;
}

/* ===== CALLBACK ===== */

static void on_valider(GtkButton *b, gpointer d) {
    depart_id = get_selected_id(GTK_TREE_VIEW(tree_depart));
    arrivee_id = get_selected_id(GTK_TREE_VIEW(tree_arrivee));

    if (depart_id < 0 || arrivee_id < 0) {
        gtk_label_set_markup(GTK_LABEL(label_resultat),"<span foreground='#ff6b9d'>⚠ Sélectionnez vos stations de départ et d'arrivée</span>");
        return;
    }

    gtk_widget_destroy(selection_window);
}

static void on_restart_search(GtkButton *b, gpointer d) {
    restart_search = 1;
    gtk_widget_destroy(result_window);
}

/* ===== SELECTION DES STATIONS ===== */

int gui_choisir_stations(GuiStation *stations, int num_stations, int *depart, int *arrivee) {

    apply_css();

    /* ===== MODELE ===== */

    store_stations = gtk_list_store_new(N_COLS, G_TYPE_INT, G_TYPE_STRING);

    for (int i = 0; i < num_stations; i++) {
        GtkTreeIter iter;
        gtk_list_store_append(store_stations, &iter);
        gtk_list_store_set(store_stations, &iter,
            COL_ID, stations[i].id,
            COL_NOM, stations[i].nom,
            -1);
    }

    /* ===== FILTRES ===== */

    filter_depart = GTK_TREE_MODEL_FILTER(
        gtk_tree_model_filter_new(GTK_TREE_MODEL(store_stations), NULL));
    gtk_tree_model_filter_set_visible_func(filter_depart, filter_func_depart, NULL, NULL);

    filter_arrivee = GTK_TREE_MODEL_FILTER(
        gtk_tree_model_filter_new(GTK_TREE_MODEL(store_stations), NULL));
    gtk_tree_model_filter_set_visible_func(filter_arrivee, filter_func_arrivee, NULL, NULL);

    /* ===== FENETRE ===== */

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Planificateur de trajet en Métro");
    gtk_window_set_default_size(GTK_WINDOW(window), 1920, 1080);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    /* ===== MAIN VBOX ===== */

    GtkWidget *main_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), main_vbox);
    gtk_container_set_border_width(GTK_CONTAINER(main_vbox), 10);

    /* ===== COSMIC HEADER ===== */

    GtkWidget *header_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
    gtk_widget_set_name(header_box, "cosmic-header");

    GtkWidget *icon_label = gtk_label_new("🚇");
    gtk_widget_set_name(icon_label, "metro-icon");

    GtkWidget *title = gtk_label_new(NULL);
    gtk_widget_set_name(title, "titulo");
    gtk_label_set_markup(GTK_LABEL(title), "METRO VOYAGE");

    GtkWidget *subtitle = gtk_label_new(NULL);
    gtk_widget_set_name(subtitle, "subtitle");
    gtk_label_set_markup(GTK_LABEL(subtitle), "Planifiez votre trajet à travers la ville");

    gtk_box_pack_start(GTK_BOX(header_box), icon_label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(header_box), title, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(header_box), subtitle, FALSE, FALSE, 0);

    /* ===== CONTENT CONTAINER ===== */

    GtkWidget *content = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_name(content, "main-container");
    gtk_container_set_border_width(GTK_CONTAINER(content), 0);

    gtk_box_pack_start(GTK_BOX(content), header_box, FALSE, FALSE, 0);

    /* ===== GRID FOR STATIONS ===== */

    GtkWidget *stations_grid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(stations_grid), 20);
    gtk_grid_set_row_spacing(GTK_GRID(stations_grid), 10);
    gtk_container_set_border_width(GTK_CONTAINER(stations_grid), 10);
    gtk_widget_set_halign(stations_grid, GTK_ALIGN_CENTER);

    /* ===== DEPARTURE SECTION ===== */

    GtkWidget *depart_card = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
    gtk_widget_set_name(depart_card, "section-card");

    GtkWidget *lab_depart = gtk_label_new(NULL);
    gtk_widget_set_name(lab_depart, "section-label");
    gtk_label_set_markup(GTK_LABEL(lab_depart), "🌟 Départ");
    gtk_widget_set_halign(lab_depart, GTK_ALIGN_START);

    search_depart = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(search_depart), "🔍 Rechercher des stations...");
    g_signal_connect(search_depart, "changed", G_CALLBACK(on_search_changed_depart), NULL);

    tree_depart = gtk_tree_view_new_with_model(GTK_TREE_MODEL(filter_depart));
    GtkCellRenderer *renderer1 = gtk_cell_renderer_text_new();
    g_object_set(renderer1, "ypad", 12, "xpad", 15, NULL);
    GtkTreeViewColumn *col_depart = gtk_tree_view_column_new_with_attributes(
        "Stations", renderer1, "text", COL_NOM, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree_depart), col_depart);

    GtkWidget *scroll1 = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scroll1), GTK_SHADOW_NONE);
    gtk_container_add(GTK_CONTAINER(scroll1), tree_depart);
    gtk_widget_set_size_request(scroll1, 350, 200);

    gtk_box_pack_start(GTK_BOX(depart_card), lab_depart, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(depart_card), search_depart, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(depart_card), scroll1, TRUE, TRUE, 0);

    /* ===== ARRIVAL SECTION ===== */

    GtkWidget *arrivee_card = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
    gtk_widget_set_name(arrivee_card, "section-card");

    GtkWidget *lab_arrivee = gtk_label_new(NULL);
    gtk_widget_set_name(lab_arrivee, "section-label");
    gtk_label_set_markup(GTK_LABEL(lab_arrivee), "🎯 Arrivée");
    gtk_widget_set_halign(lab_arrivee, GTK_ALIGN_START);

    search_arrivee = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(search_arrivee), "🔍 Rechercher des stations...");
    g_signal_connect(search_arrivee, "changed", G_CALLBACK(on_search_changed_arrivee), NULL);

    tree_arrivee = gtk_tree_view_new_with_model(GTK_TREE_MODEL(filter_arrivee));
    GtkCellRenderer *renderer2 = gtk_cell_renderer_text_new();
    g_object_set(renderer2, "ypad", 12, "xpad", 15, NULL);
    GtkTreeViewColumn *col_arrivee = gtk_tree_view_column_new_with_attributes(
        "Stations", renderer2, "text", COL_NOM, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree_arrivee), col_arrivee);

    GtkWidget *scroll2 = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scroll2), GTK_SHADOW_NONE);
    gtk_container_add(GTK_CONTAINER(scroll2), tree_arrivee);
    gtk_widget_set_size_request(scroll2, 350, 200);

    gtk_box_pack_start(GTK_BOX(arrivee_card), lab_arrivee, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(arrivee_card), search_arrivee, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(arrivee_card), scroll2, TRUE, TRUE, 0);

    /* ===== ADD TO GRID ===== */

    gtk_grid_attach(GTK_GRID(stations_grid), depart_card, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(stations_grid), arrivee_card, 1, 0, 1, 1);

    gtk_box_pack_start(GTK_BOX(content), stations_grid, FALSE, FALSE, 0);

    /* ===== BUTTON ===== */

    GtkWidget *btn_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
    gtk_container_set_border_width(GTK_CONTAINER(btn_box), 30);

    GtkWidget *btn = gtk_button_new_with_label("✨ Trouver mon Chemin ✨");
    gtk_widget_set_name(btn, "btn-validate");
    gtk_widget_set_halign(btn, GTK_ALIGN_CENTER);
    g_signal_connect(btn, "clicked", G_CALLBACK(on_valider), NULL);

    label_resultat = gtk_label_new("");
    gtk_widget_set_name(label_resultat, "error-label");

    gtk_box_pack_start(GTK_BOX(btn_box), btn, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(btn_box), label_resultat, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(content), btn_box, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(main_vbox), content, TRUE, TRUE, 0);

    selection_window = window;
    depart_id = -1;
    arrivee_id = -1;
    gtk_widget_show_all(window);

    return 1;
}

/* ===== AFFICHAGE DE LA SOLUTION ===== */

void gui_afficher_solution(resultat_t res) {

    apply_css();

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Votre Voyage en Metro");
    gtk_window_set_default_size(GTK_WINDOW(window), 1920, 1080);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    /* ===== MAIN CONTAINER ===== */

    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), main_box);
    gtk_container_set_border_width(GTK_CONTAINER(main_box), 5);

    GtkWidget *content = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_name(content, "result-container");

    /* ===== JOURNEY HEADER ===== */

    GtkWidget *journey_header = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_widget_set_name(journey_header, "journey-header");

    GtkWidget *header_title = gtk_label_new(NULL);
    gtk_widget_set_name(header_title, "titulo");
    gtk_label_set_markup(GTK_LABEL(header_title), "Votre Voyage");

    /* ===== STATIONS DISPLAY ===== */

    GtkWidget *stations_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 15);
    gtk_box_set_homogeneous(GTK_BOX(stations_box), TRUE);

    /* Departure */
    GtkWidget *depart_badge = gtk_box_new(GTK_ORIENTATION_VERTICAL, 3);
    gtk_widget_set_name(depart_badge, "station-badge");

    GtkWidget *depart_label = gtk_label_new("De");
    gtk_widget_set_name(depart_label, "station-label");

    GtkWidget *depart_name = gtk_label_new(NULL);
    gtk_widget_set_name(depart_name, "station-name");
    gtk_label_set_markup(GTK_LABEL(depart_name), res.noms_stations[0]);
    gtk_label_set_line_wrap(GTK_LABEL(depart_name), TRUE);
    gtk_label_set_max_width_chars(GTK_LABEL(depart_name), 20);

    gtk_box_pack_start(GTK_BOX(depart_badge), depart_label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(depart_badge), depart_name, FALSE, FALSE, 0);

    /* Arrow */
    GtkWidget *arrow = gtk_label_new("→");
    gtk_widget_set_name(arrow, "glow-arrow");

    /* Arrival */
    GtkWidget *arrival_badge = gtk_box_new(GTK_ORIENTATION_VERTICAL, 3);
    gtk_widget_set_name(arrival_badge, "station-badge");

    GtkWidget *arrival_label = gtk_label_new("À");
    gtk_widget_set_name(arrival_label, "station-label");

    GtkWidget *arrival_name = gtk_label_new(NULL);
    gtk_widget_set_name(arrival_name, "station-name");
    gtk_label_set_markup(GTK_LABEL(arrival_name), res.noms_stations[res.longueur - 1]);
    gtk_label_set_line_wrap(GTK_LABEL(arrival_name), TRUE);
    gtk_label_set_max_width_chars(GTK_LABEL(arrival_name), 20);

    gtk_box_pack_start(GTK_BOX(arrival_badge), arrival_label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(arrival_badge), arrival_name, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(stations_box), depart_badge, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(stations_box), arrow, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(stations_box), arrival_badge, TRUE, TRUE, 0);

    gtk_box_pack_start(GTK_BOX(journey_header), header_title, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(journey_header), stations_box, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(content), journey_header, FALSE, FALSE, 0);

    /* ===== TIME CARD ===== */

    char temps_txt[256];
    snprintf(temps_txt, sizeof(temps_txt), "⏱ %d minutes", res.temps_total);

    GtkWidget *time_card = gtk_label_new(NULL);
    gtk_widget_set_name(time_card, "time-card");
    gtk_label_set_markup(GTK_LABEL(time_card), temps_txt);
    gtk_widget_set_halign(time_card, GTK_ALIGN_CENTER);

    GtkWidget *time_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_set_border_width(GTK_CONTAINER(time_box), 5);
    gtk_box_pack_start(GTK_BOX(time_box), time_card, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(content), time_box, FALSE, FALSE, 0);

    /* ===== SEPARATOR ===== */
    GtkWidget *sep1 = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_pack_start(GTK_BOX(content), sep1, FALSE, FALSE, 5);

    /* ===== DETAILS TITLE ===== */
    GtkWidget *details_title = gtk_label_new(NULL);
    gtk_widget_set_name(details_title, "Details titre");
    gtk_label_set_markup(GTK_LABEL(details_title), "Details du voyage");
    gtk_box_pack_start(GTK_BOX(content), details_title, FALSE, FALSE, 5);

    /* ===== TABLE ===== */

    GtkListStore *store = gtk_list_store_new(3, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);

    for (int i = 0; i < res.longueur; i++) {
        GtkTreeIter iter;
        gtk_list_store_append(store, &iter);

        char num[16];
        snprintf(num, sizeof(num), "%d", i + 1);

        char ligne[64];
        if (i == 0) {
            strcpy(ligne, "🟢 Début");
        } else if (i == res.longueur - 1) {
            strcpy(ligne, "🔴 fin");
        } else if (strlen(res.lignes[i - 1]) > 0) {
            snprintf(ligne, sizeof(ligne), "Ligne %s", res.lignes[i - 1]);
        } else {
            strcpy(ligne, "—");
        }

        gtk_list_store_set(store, &iter, 0, num, 1, res.noms_stations[i], 2, ligne, -1);
    }

    GtkWidget *tree = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));

    GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
    g_object_set(renderer, "ypad", 8, "xpad", 10, NULL);

    GtkTreeViewColumn *col1 = gtk_tree_view_column_new_with_attributes(
        "Etape", renderer, "text", 0, NULL);
    gtk_tree_view_column_set_fixed_width(col1, 120);

    GtkTreeViewColumn *col2 = gtk_tree_view_column_new_with_attributes(
        "Station", renderer, "text", 1, NULL);
    gtk_tree_view_column_set_expand(col2, TRUE);

    GtkTreeViewColumn *col3 = gtk_tree_view_column_new_with_attributes(
        "Ligne", renderer, "text", 2, NULL);
    gtk_tree_view_column_set_fixed_width(col3, 150);

    gtk_tree_view_append_column(GTK_TREE_VIEW(tree), col1);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree), col2);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree), col3);

    GtkWidget *scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scroll), GTK_SHADOW_NONE);
    gtk_widget_set_vexpand(scroll, TRUE);
    gtk_widget_set_size_request(scroll, -1, 120);
    gtk_container_add(GTK_CONTAINER(scroll), tree);
    gtk_container_set_border_width(GTK_CONTAINER(scroll), 3);

    gtk_box_pack_start(GTK_BOX(content), scroll, TRUE, TRUE, 0);

    /* ===== SEPARATOR ===== */
    GtkWidget *sep2 = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_pack_start(GTK_BOX(content), sep2, FALSE, FALSE, 5);

    /* ===== BUTTONS ===== */
    GtkWidget *btn_restart = gtk_button_new_with_label("🔄 Nouvel Itinéraire");
    gtk_widget_set_name(btn_restart, "btn-restart");
    g_signal_connect(btn_restart, "clicked", G_CALLBACK(on_restart_search), NULL);

    GtkWidget *btn_close = gtk_button_new_with_label("❌ Fermer");
    gtk_widget_set_name(btn_close, "btn-close");
    g_signal_connect(btn_close, "clicked", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *btn_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_widget_set_halign(btn_hbox, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(btn_hbox), btn_restart, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(btn_hbox), btn_close, FALSE, FALSE, 0);

    GtkWidget *btn_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_set_border_width(GTK_CONTAINER(btn_box), 5);
    gtk_box_pack_start(GTK_BOX(btn_box), btn_hbox, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(content), btn_box, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(main_box), content, TRUE, TRUE, 0);

    result_window = window;
    gtk_widget_show_all(window);
}

/* ===== GETTERS POUR GESTION D'ETAT ===== */

int gui_get_depart_id(void) {
    return depart_id;
}

int gui_get_arrivee_id(void) {
    return arrivee_id;
}

int gui_get_restart_search(void) {
    return restart_search;
}
