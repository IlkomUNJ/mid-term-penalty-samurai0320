#include "drawingcanvas.h"
using namespace std;

DrawingCanvas::DrawingCanvas(QWidget *parent)  {
    // Set a minimum size for the canvas
    setMinimumSize(this->WINDOW_WIDTH, this->WINDOW_HEIGHT);
    // Set a solid background color
    setStyleSheet("background-color: white; border: 1px solid gray;");
}

void DrawingCanvas::clearPoints(){
    m_points.clear();
    // Trigger a repaint to clear the canvas
    m_patternFrequency.clear();
    m_segmentCandidates.clear();
    isPaintLinesClicked = false;
    update();
}

void DrawingCanvas::paintLines(){
    /* Todo
     * Implement lines drawing per even pair
    */

    isPaintLinesClicked = true;
    update();
}

void DrawingCanvas::segmentDetection(){
    m_patternFrequency.clear();
    m_segmentCandidates.clear();
    QPixmap pixmap = this->grab(); //
    QImage image = pixmap.toImage();


    cout << " Deteksi Segmen " << endl;
    cout << "image width " << image.width() << endl;
    cout << "image height " << image.height() << endl;

    //To not crash we set initial size of the matrix
    vector<CustomMatrix> windows(image.width()*image.height());

    // Get the pixel value as an ARGB integer (QRgb is a typedef for unsigned int)
    for(int i = 1; i < image.width()-1;i++){
        for(int j = 1; j < image.height()-1;j++){
            bool local_window[3][3] = {false};

            for(int m=-1;m<=1;m++){
                for(int n=-1;n<=1;n++){
                    local_window[m+1][n+1] = (image.pixel(i+m, j+n) != 0xffffffff);
                }
            }
            CustomMatrix currentMatrix(local_window);
            string key = currentMatrix.toString();
            m_patternFrequency[key]++;
            if(currentMatrix.isCenterSet() && currentMatrix.getNeighborCount() == 2){
                m_segmentCandidates.push_back(QPoint(i, j));
            }


            //CustomMatrix mat(local_window);

            //windows.push_back(mat);
        }
    }
    cout << "\n--- Kandidat Segmen (Tugas 3) ---" << endl;
    cout << "Ditemukan " << m_segmentCandidates.size() << " potensial kandidat." << endl;
    for (const QPoint& point : m_segmentCandidates) {
        cout << "Kandidat di: (" << point.x() << ", " << point.y() << ")" << endl;
    }
    cout << "\n--- Frekuensi Pola (Tugas 2) ---" << endl;
    cout << "Total pola unik ditemukan: " << m_patternFrequency.size() << endl;
    for (const auto& pair : m_patternFrequency) {
        string patternKey = pair.first;
        int frequency = pair.second;
        cout << "\nPola (Ditemukan " << frequency << " kali):" << endl;
        cout << PrintPattern(patternKey);
    }

    reportPatterns();
    update();
}
void DrawingCanvas::reportPatterns(){
    cout << "Laporan Frekuensi Pola" << endl;
    cout << "Total pola ditemukan: " << m_patternFrequency.size() << endl;

    std::multimap<int, string, std::greater<int>> sortedPatterns;
    for(const auto& pair : m_patternFrequency){
        sortedPatterns.insert({pair.second, pair.first});
    }

    int count = 0;
    cout << "Menampilkan 20 pola teratas:" << endl;
    for(const auto& pair : sortedPatterns){
        if(count++ >= 20) {
            cout << " dan sisa " << (sortedPatterns.size() - count + 1) << " pola lainnya." << endl;
            break;
        }
        cout << " " << endl;
        cout << "Jumlah: " << pair.first << endl;
        cout << "Pola: " << endl;
        cout << PrintPattern(pair.second);
    }
}

void DrawingCanvas::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Set up the pen and brush for drawing the points
    QPen pen(Qt::blue, 5);
    painter.setPen(pen);
    painter.setBrush(QBrush(Qt::blue));

    // Draw a small circle at each stored point
    for (const QPoint& point : std::as_const(m_points)) {
        painter.drawEllipse(point, 3, 3);
    }

    if(isPaintLinesClicked){
        cout << "paint lines block is called" << endl;
        pen.setColor(Qt::red);
        pen.setWidth(4); // 4-pixel wide line
        pen.setStyle(Qt::SolidLine);
        painter.setPen(pen);

        // Set the painter's pen to our custom pen.

        //for(int i=0;i<m_points.size()-1;i+=2){
        //cout << m_points[i].x() << endl;
        //painter.drawLine(m_points[i], m_points[i+1]);
        //}
        for(int i=0;i<m_points.size()-1;i+=2){
            painter.drawLine(m_points[i], m_points[i+1]);
        }
        isPaintLinesClicked = false;

        //return painter pen to blue
        pen.setColor(Qt::blue);
        painter.setPen(pen);
    }
    if(!m_segmentCandidates.isEmpty()){
        pen.setColor(Qt::green);
        pen.setWidth(1);
        painter.setPen(pen);
        for(const QPoint& point : std::as_const(m_segmentCandidates)){
            painter.drawPoint(point);
        }
    }
}

void DrawingCanvas::mousePressEvent(QMouseEvent *event) {
    // Add the mouse click position to our vector of points
    m_points.append(event->pos());
    // Trigger a repaint
    update();
}

string DrawingCanvas::PrintPattern(string key){
    stringstream ss;
    for(int i=0; i<9; i++){
        ss << key[i] ;
        if((i+1) % 3 == 0) ss << endl;
    }
    return ss.str();
}


