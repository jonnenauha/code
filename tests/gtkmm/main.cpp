/* main.cpp -- main module
 *
 *			Ryan McDougall -- 20070101
 */

#include <iostream>
#include <memory>
#include <gtkmm.h>
#include <cairomm/cairomm.h>

using namespace std;

static const int NCHANNELS (3); // RGB

template <typename T>
class RenderBuffer
{
    public:
        RenderBuffer () 
            : size_ (0), allocsize_ (0), buf_ (NULL) 
        {}

        RenderBuffer (int n) 
            : size_ (0), allocsize_ (0), buf_ (NULL) 
        {}

        T* begin () { return buf_.get(); }
        T* end () { return buf_.get()+size_; }

        int size () { return size_; }
        int stride () { return stride_; }
        int channel () { return channel_; }
        int width () { return width_; }
        int height () { return height_; }

        void resize (int n, int stride, int nchan) 
        { 
            width_ = 0; // invalidate
            height_ = 0; // invalidate

            size_ = n; 
            stride_ = stride;
            channel_ = nchan;

            if (size_ > allocsize_)
            {
                allocsize_ = (size_ > (allocsize_ << 1))? 
                    size_ : allocsize_ << 1;

                buf_.reset (new T [allocsize_]);
            }
        }

        void resize_raster (int width, int height, int nchan)
        {
            resize (width * height * nchan, width * nchan, nchan);
            width_ = width;
            height_ = height;
        }

    private:
        int width_;
        int height_;
        int stride_;
        int channel_;

        int size_;
        int allocsize_;

        auto_ptr <T> buf_;

    private:
        RenderBuffer (const RenderBuffer&);
        void operator= (const RenderBuffer&);
};

class RenderArea : public Gtk::DrawingArea
{
    public:
        RenderArea () { }
        virtual ~RenderArea () { }

    protected:
        virtual bool on_expose_event (GdkEventExpose *event)
        {
            cout << "expose!" << endl;

            int nchan (buf_.channel());
            int stride (buf_.stride());

            guint8 *pixel (buf_.begin()), *bufend (buf_.end());
            guint8 *row (pixel), *rowend (pixel + stride);

            while (row < bufend)
            {
                for (pixel = row; pixel < rowend; pixel += nchan)
                {
                    pixel[0] = 0xFF;
                    pixel[1] = 0x00;
                    pixel[2] = 0x00;
                }

                row += stride;
                rowend += stride;
            }

            get_window()-> draw_rgb_image 
                (get_style()-> get_fg_gc (get_state()), 
                 0, 0, buf_.width(), buf_.height(), 
                 Gdk::RGB_DITHER_NONE, buf_.begin(), buf_.stride());

            return true;
        }

        virtual bool on_configure_event (GdkEventConfigure *event)
        {
            buf_.resize_raster (event-> width, event-> height, NCHANNELS);
        }

    private:
        RenderBuffer <guint8> buf_;

    private:
        RenderArea (const RenderArea&);
        void operator= (const RenderArea&);
};

class AppWindow : public Gtk::Window
{
    public:
        AppWindow () 
        {
            set_title ("play renderer");
            set_default_size (200, 200);
            add (packingbox_);

            setup_menu_();
            setup_drawing_area_();

            show();
            show_all_children();
        }

        virtual ~AppWindow () 
        {
        }

    protected:
        // signal handlers
        virtual void on_quit ()
        {
            hide(); // closes this window, which causes Gtk::main::run() to terminate
        }

    private:

        Gtk::VBox packingbox_;
        Glib::RefPtr<Gtk::UIManager> uimanager_;
        Glib::RefPtr<Gtk::ActionGroup> actiongroup_;

        RenderArea renderarea_;

        void setup_menu_ ()
        {
            actiongroup_ = Gtk::ActionGroup::create ();
            actiongroup_-> add (Gtk::Action::create ("FileMenu", "File"));

            actiongroup_-> add 
                (Gtk::Action::create ("FileQuit", Gtk::Stock::QUIT), 
                 Gtk::AccelKey ("<control>Q"),
                 sigc::mem_fun (*this, &AppWindow::on_quit));

            uimanager_ = Gtk::UIManager::create ();
            uimanager_-> insert_action_group (actiongroup_);

            add_accel_group (uimanager_-> get_accel_group());

            Glib::ustring uiinfo =         
                "<ui>"
                "  <menubar name='MenuBar'>"
                "    <menu action='FileMenu'>"
                "      <separator/>"
                "      <menuitem action='FileQuit'/>"
                "    </menu>"
                "  </menubar>"
                "  <toolbar  name='ToolBar'>"
                "    <toolitem action='FileQuit'/>"
                "  </toolbar>"
                "</ui>";

            try 
            {
                uimanager_-> add_ui_from_string (uiinfo);
            }
            catch (const Glib::Error& ex)
            {
                std::cerr << "building menus failed: " << ex.what() << "\n";
            }

            Gtk::Widget *menubar (uimanager_-> get_widget ("/MenuBar"));
            Gtk::Widget *toolbar (uimanager_-> get_widget ("/ToolBar"));

            if (menubar) packingbox_.pack_start (*menubar, Gtk::PACK_SHRINK);
            if (toolbar) packingbox_.pack_start (*toolbar, Gtk::PACK_SHRINK);
        }

        void setup_drawing_area_ ()
        {
            packingbox_.add (renderarea_);
        }

    private:
        AppWindow (const AppWindow&);
        void operator= (const AppWindow&);
};

//=============================================================================
// Main entry point
int
main (int argc, char** argv)
{
    Gtk::Main app (argc, argv);
    AppWindow win;

    Gtk::Main::run (win);

    return 0;
}

