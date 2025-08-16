namespace StereoscopicMoviePlayer
{
    partial class SoundPanel
    {
        /// <summary> 
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary> 
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.tbVolume = new System.Windows.Forms.TrackBar();
            this.bSound = new System.Windows.Forms.Button();
            this.cbTracks = new System.Windows.Forms.ComboBox();
            this.ttControls = new System.Windows.Forms.ToolTip(this.components);
            ((System.ComponentModel.ISupportInitialize)(this.tbVolume)).BeginInit();
            this.SuspendLayout();
            // 
            // tbVolume
            // 
            this.tbVolume.AutoSize = false;
            this.tbVolume.Location = new System.Drawing.Point(26, 2);
            this.tbVolume.Maximum = 65535;
            this.tbVolume.Name = "tbVolume";
            this.tbVolume.Size = new System.Drawing.Size(112, 23);
            this.tbVolume.TabIndex = 45;
            this.tbVolume.TickFrequency = 5000;
            this.tbVolume.Scroll += new System.EventHandler(this.tbVolume_Scroll);
            this.tbVolume.MouseEnter += new System.EventHandler(this.tbVolume_MouseEnter);
            // 
            // bSound
            // 
            this.bSound.BackgroundImage = global::StereoscopicMoviePlayer.Properties.Resources.sound;
            this.bSound.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
            this.bSound.FlatAppearance.BorderSize = 0;
            this.bSound.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.bSound.Location = new System.Drawing.Point(0, 0);
            this.bSound.Name = "bSound";
            this.bSound.Size = new System.Drawing.Size(26, 26);
            this.bSound.TabIndex = 47;
            this.bSound.UseVisualStyleBackColor = true;
            this.bSound.Click += new System.EventHandler(this.bSound_Click);
            this.bSound.MouseEnter += new System.EventHandler(this.bSound_MouseEnter);
            // 
            // cbTracks
            // 
            this.cbTracks.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cbTracks.FormattingEnabled = true;
            this.cbTracks.Location = new System.Drawing.Point(139, 3);
            this.cbTracks.Name = "cbTracks";
            this.cbTracks.Size = new System.Drawing.Size(70, 21);
            this.cbTracks.TabIndex = 46;
            this.cbTracks.SelectedIndexChanged += new System.EventHandler(this.cbTracks_SelectedIndexChanged);
            this.cbTracks.MouseEnter += new System.EventHandler(this.cbTracks_MouseEnter);
            // 
            // ttControls
            // 
            this.ttControls.AutoPopDelay = 5000;
            this.ttControls.InitialDelay = 500;
            this.ttControls.ReshowDelay = 100;
            // 
            // SoundPanel
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.tbVolume);
            this.Controls.Add(this.bSound);
            this.Controls.Add(this.cbTracks);
            this.Name = "SoundPanel";
            this.Size = new System.Drawing.Size(213, 26);
            ((System.ComponentModel.ISupportInitialize)(this.tbVolume)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TrackBar tbVolume;
        private System.Windows.Forms.Button bSound;
        private System.Windows.Forms.ComboBox cbTracks;
        private System.Windows.Forms.ToolTip ttControls;
    }
}